@;-----------------------------------------------------------------------
@;  E9M22_s.s: operacions amb números en format Coma Flotant E9M22.
@;-----------------------------------------------------------------------
@;	santiago.romani@urv.cat
@;	pere.millan@urv.cat
@;	(Març 2021-2023, Febrer 2024, Març 2025)
@;-----------------------------------------------------------------------
@;	Programador/a 1: guillem.requeno@estudiants.urv.cat
@;	Programador/a 2: alexxavier.jurado@estudiants.urv.cat
@;-----------------------------------------------------------------------

.include "E9M22.i"

FLOAT_sNAN	=	0x7FA00000	@; Un possible NaN (signaling) en binary32

.text
.alinear 2
.arm

@;******************************************************
@;* Rutines de CONVERSIÓ de valors E9M22 <-> float/int *
@;******************************************************

@; E9M22_to_float_s(): converteix un valor E9M22 a float.
@;	Entrada:
@;		input 	-> R0: valor E9M22 a convertir
@;	Sortida:
@;		R0 		-> valor E9M22 convertit a float.

.global E9M22_to_float_s
E9M22_to_float_s:
		push {lr}			@; Apila l'adreça de retorn

		ldr r0, =FLOAT_sNAN		@; Carrega a R0 el valor de FLOAT_sNAN (NaN) per indicar rutina pendent
	
		pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; float_to_E9M22_s(): converteix un valor float a E9M22.
@;	Entrada:
@;		input 	-> R0: valor float a convertir
@;	Sortida:
@;		R0 		-> valor float convertit a E9M22.

.global float_to_E9M22_s
float_to_E9M22_s:
		push {lr}			@; Apila l'adreça de retorn

		ldr r0, =E9M22_sNAN		@; Carrega a R0 el valor de E9M22_sNAN (NaN) per indicar rutina pendent
	
		pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; E9M22_to_int_s(): converteix un valor E9M22 a enter (arrodonit).
@;	Entrada:
@;		input 	-> R0: valor E9M22 a convertir
@;	Sortida:
@;		R0 		-> valor E9M22 convertit a enter.

.global E9M22_to_int_s
E9M22_to_int_s:
		push {lr}			@; Apila l'adreça de retorn

		mov r0, #-1		@; Carrega -1 a R0 per indicar rutina pendent
	
		pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; int_to_E9M22_s(): converteix un valor enter a E9M22.
@;	Entrada:
@;		input 	-> R0: valor enter a convertir
@;	Sortida:
@;		R0 		-> valor float convertit a E9M22.

.global int_to_E9M22_s
int_to_E9M22_s:
		push {lr}			@; Apila l'adreça de retorn

		ldr r0, =E9M22_sNAN		@; Carrega a R0 el valor de E9M22_sNAN (NaN) per indicar rutina pendent
	
		pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@;*************************************************
@;* Operacions ARITMÈTIQUES en Coma Flotant E9M22 *
@;*************************************************

@; E9M22_add_s(): calcula i retorna la suma dels 2 operands,
@;                (num1 + num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 + num2).

.global E9M22_add_s

E9M22_add_s:
	push {r4-r12, lr}	@; Apila registres callee-saved i l'adreça de retorn

	@; constants i màscares
	ldr r2, =0x80000000      @; r2 = mask_sign (bit de signe)
	ldr r3, =0x7fc00000      @; r3 = mask_exp (bits de l'exponent)
	ldr r4, =0x003fffff      @; r4 = mask_frac (bits de la fracció o mantissa)
	ldr r5, =0x7fc00000      @; r5 = exponent tot 1s per detectar nan/inf

	@; guardem operands
	mov r6, r0               @; r6 = num1
	mov r7, r1               @; r7 = num2

	@; comprovar si num1 és nan
	and r8, r6, r3           @; r8 = exponent de num1
	cmp r8, r5               @; és exponent tot 1s?
	bne comprovar_nan2	   @; Si no ho és, salta a comprovar num2
	and r9, r6, r4           @; r9 = fracció de num1
	cmp r9, #0
	bne return_num1          @; Si la fracció no és zero, num1 és NaN, retorna num1

comprovar_nan2:
	and r8, r7, r3
	cmp r8, r5
	bne comprovar_infinit1
	and r9, r7, r4
	cmp r9, #0
	bne return_num2          @; num2 és nan

comprovar_infinit1:
	and r8, r6, r3
	cmp r8, r5
	bne comprovar_infinit2
	and r9, r6, r4
	cmp r9, #0
	bne comprovar_infinit2           @; no és inf real

	and r10, r7, r3
	cmp r10, r5
	bne return_num1          @; num1 = inf, num2 finit -> retorna num1

	and r11, r7, r4
	cmp r11, #0
	bne return_num1          @; num2 = inf

	@; tots dos són inf, comparar signes
	and r8, r6, r2
	and r9, r7, r2
	cmp r8, r9
	beq return_num1
	ldr r0, =0x7fffffff      @; qnan fictici
	b end

comprovar_infinit2:
	and r8, r7, r3
	cmp r8, r5
	bne comprovar_zeros
	and r9, r7, r4
	cmp r9, #0
	bne comprovar_zeros
	b return_num2            @; num2 = inf

comprovar_zeros:
	and r8, r6, #0x7fffffff
	cmp r8, #0	
	beq return_num2          @; num1 és zero

	and r8, r7, #0x7fffffff
	cmp r8, #0
	beq return_num1          @; num2 és zero

	@; decodificar num1
	and r8, r6, r3           @; exponent num1
	mov r9, r6               @; signe1 = bit 31
	and r9, r9, r2
	cmp r8, #0
	beq denormal1
	lsr r10, r8, #22
	sub r10, r10, #255       @; exp1
	orr r11, r6, #0x00400000 @; mant1 = 1.|frac
	b dec2

denormal1:
	mov r10, #-254           @; exp1 denormal
	and r11, r6, r4          @; mant1

dec2:
	and r8, r7, r3
	mov r12, r7              @; signe2
	and r12, r12, r2
	cmp r8, #0
	beq denormal2
	lsr r8, r8, #22
	sub r8, r8, #255         @; exp2
	orr r14, r7, #0x00400000 @; mant2
	b alinear
denormal2:
	mov r8, #-254
	and r14, r7, r4

alinear:
	mov r0, r10              @; exp1
	cmp r0, r8
	beq alineada

	bgt exponent1
	sub r1, r8, r0
	cmp r1, #9
	bge zero_mant1
	lsl r14, r14, r1
	sub r8, r8, r1
	b alineada

exponent1:
	sub r1, r0, r8
	cmp r1, #9
	bge zero_mant2
	lsl r11, r11, r1
	sub r10, r10, r1
	b alineada

zero_mant1:
	mov r11, #0
	mov r10, r8
	b alineada

zero_mant2:
	mov r14, #0
	mov r8, r10

alineada:
	@; suma mantisses amb signe
	cmp r9, r12
	beq mateix_signe

	@; signes diferents, aplicar signe negatiu
	cmp r9, #0
	bne negat_mantissa1
	rsbs r14, r14, #0        @; -mant2
	b suma_mantissa

negat_mantissa1:
	rsbs r11, r11, #0        @; -mant1

suma_mantissa:
	add r0, r11, r14         @; mant_suma
	cmp r0, #0
	bmi signe_negatiu
	mov r1, #0               @; signe_suma = positiu
	b normalitzacio

signe_negatiu:
	rsbs r0, r0, #0
	mov r1, #0x80000000      @; signe negatiu

normalitzacio:
	@; normalització bàsica: cerca primer '1' a mantissa
	clz r2, r0
	sub r2, r2, #8           @; offset bits mantissa
	cmp r2, #0
	ble preparar_mantissa
	lsl r0, r0, r2
	sub r10, r10, r2         @; restem a exponent

preparar_mantissa:
	@; comprovar overflow exponent
	add r10, r10, #255
	ldr r3, =0x1ff
	cmp r10, r3
	ldr r4, =0x7f800000
	movgt r0, r4    @; inf
	orrgt r0, r0, r1
	bgt end

	cmp r10, #0
	movlt r0, #0             @; zero per sota del mínim
	orrlt r0, r0, r1
	blt end

	lsl r0, r0, #1           @; eliminem bit ocult
	lsr r0, r0, #10          @; ajust a 22 bits
	lsl r10, r10, #22
	orr r0, r0, r10
	orr r0, r0, r1
	b end

mateix_signe:
	add r0, r11, r14         @; mant_suma
	mov r1, r9               @; signe_suma
	b normalitzacio

return_num1:
	mov r0, r6
	b end

return_num2:
	mov r0, r7
	b end

end:
	pop {r4-r12, pc}	@; Desapila registres i salta a l'adreça de retorn

@; E9M22_sub_s(): calcula i retorna la diferència dels 2 operands,
@;                (num1 - num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 - num2).

.global E9M22_sub_s
E9M22_sub_s:
	push {r4, lr}			@; Apila r4 i l'adreça de retorn

    @; Validem si algun dels operands és NaN
    ldr r4, =E9M22_qNAN
    cmp r0, r4              @; Comprovem si num1 és NaN
    beq return_nan          @; Si num1 és NaN, retornem NaN
    cmp r1, r4              @; Comprovem si num2 és NaN
    beq return_nan          @; Si num2 és NaN, retornem NaN

    @; Neguem el segon operand (num2) cridant E9M22_neg_s
    mov r4, r0              @; Guardem num1 a R4 per preservar-lo temporalment
    mov r0, r1              @; Movem num2 a R0 per preparar la crida a E9M22_neg_s
    bl E9M22_neg_s          @; Cridem E9M22_neg_s per obtenir -num2
    mov r1, r0              @; Movem -num2 (R0) a R1 per preparar la crida a E9M22_add_s
    mov r0, r4              @; Recuperem num1 a R0 des de R4

    @; Sumem num1 i -num2 cridant E9M22_add_s
    bl E9M22_add_c          @; Cridem E9M22_add_s per calcular (num1 + (-num2))

    pop {r4, pc}            @; Restaurem r4 i retornem

return_nan:
	mov r0, r4              @; Retornem NaN
	pop {r4, pc}

@; E9M22_mul_s(): calcula i retorna el producte dels 2 operands,
@;                (num1 × num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 × num2).

.global E9M22_mul_s
E9M22_mul_s:
	push {lr}			@; Apila l'adreça de retorn

	ldr r0, =E9M22_sNAN		@; Carrega a R0 el valor de E9M22_sNAN (NaN) per indicar rutina pendent
	
	pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; E9M22_div_s(): calcula i retorna la divisió dels 2 operands,
@;                (num1 ÷ num2) codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> valor E9M22 de (num1 ÷ num2).

.global E9M22_div_s
E9M22_div_s:
	push {lr}			@; Apila l'adreça de retorn

	ldr r0, =E9M22_sNAN		@; Carrega a R0 el valor de E9M22_sNAN (NaN) per indicar rutina pendent
	
	pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; E9M22_neg_s(): canvia el signe (nega) de l'operand num
@;                codificat en coma flotant E9M22.
@;	Entrada:
@;		num 	-> R0: valor a negar
@;	Sortida:
@;		R0 		-> valor E9M22 negat de num.

.global E9M22_neg_s

E9M22_neg_s:
	push {lr}				@; Apila l'adreça de retorn

    @; Canviem el signe amb XOR
    eor r0, r0, #E9M22_MASK_SIGN	@; Invertim el bit de signe

    pop {pc}				@; Desapila l'adreça de retorn i salta-hi

@; E9M22_abs_s(): calcula i retorna el valor absolut
@;                de l'operand num codificat en coma flotant E9M22.
@;	Entrada:
@;		num 	-> R0: valor a calcular el valor absolut
@;	Sortida:
@;		R0 		-> valor absolut E9M22 de num.

.global E9M22_abs_s
E9M22_abs_s:
	push {lr}				@; Apila l'adreça de retorn

    @; Màscara per posar el bit de signe a 0
	@; La mascara són tots els bits a 1, meny el de signe
    and r0, r0, #0x7FFFFFFF	@; Força el bit de signe a 0 amb una màscara

    pop {pc}						@; Desapila l'adreça de retorn i salta-hi

@;*************************************************************
@;* Operacions de COMPARACIÓ de números en Coma Flotant E9M22 *
@;*************************************************************

@; E9M22_are_eq_s(): indica si num1 == num2, considerant valors
@;                   codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 == num2.

.global E9M22_are_eq_s
E9M22_are_eq_s:
	push {r4, lr}				@; Apila r4 i l'adreça de retorn

    @; Comprovem si num1 o num2 és NaN
    ldr r4, =E9M22_MASK_EXP         @; Carreguem la màscara de l'exponent
    tst r0, r4                      @; Comprovem si num1 té tots els bits de l'exponent a 1
    bne comprovar_nan_num1          @; Si no tots els bits són 1, saltem
    ldr r4, =E9M22_MASK_FRAC        @; Carreguem la màscara de la mantissa
    tst r0, r4                      @; Comprovem si num1 té alguna fracció diferent de 0 (és NaN)
    beq no_igual                    @; Si és NaN, saltem per retornar fals

comprovar_nan_num1:
	ldr r4, =E9M22_MASK_EXP         @; Carreguem la màscara de l'exponent
	tst r1, r4                      @; Comprovem si num2 té tots els bits de l'exponent a 1
	bne comprovar_zero              @; Si no tots els bits són 1, saltem
	ldr r4, =E9M22_MASK_FRAC        @; Carreguem la màscara de la mantissa
	tst r1, r4                      @; Comprovem si num2 té alguna fracció diferent de 0 (és NaN)
	beq no_igual                    @; Si és NaN, saltem per retornar fals

comprovar_zero:
	@; Comprovem si num1 i num2 són zeros (+0.0 o -0.0)
	ldr r4, =E9M22_MASK_SIGN        @; Carreguem la màscara del signe	
	mvn r4, r4                      @; Neguem la màscara per obtenir ~E9M22_MASK_SIGN
	and r5, r0, r4                  @; Eliminem el bit de signe de num1
	cmp r5, #0                      @; Comprovem si num1 és zero
	bne comprovar_normal            @; Si no és zero, saltem
	and r5, r1, r4                  @; Eliminem el bit de signe de num2
	cmp r5, #0                      @; Comprovem si num2 és zero
	beq igual                       @; Si tots dos són zeros, són iguals

comprovar_normal:
	@; Comparació normal
	cmp r0, r1                      @; Comparem num1 i num2 directament
	beq igual                       @; Si són iguals, saltem per retornar cert

no_igual:
	mov r0, #0                      @; Retornem 0 (fals)
	b fi                            @; Anem al final

igual:
	mov r0, #1                      @; Retornem 1 (cert)

fi:
	pop {r4, pc}                    @; Restaurem els registres i tornem al punt de crida

@; E9M22_are_ne_s(): indica si num1 ≠ num2, considerant valors
@;                   codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 ≠ num2.

.global E9M22_are_ne_s
E9M22_are_ne_s:
	push {lr}				@; Apila l'adreça de retorn

    @; Cridem E9M22_are_eq_s per comprovar si num1 == num2
    bl E9M22_are_eq_s		@; Comprovem si num1 == num2
    cmp r0, #0				@; Comparem el resultat amb 0
    moveq r0, #1			@; Si són iguals (r0 = 0), tornem 1 (≠)
    movne r0, #0			@; Si són diferents (r0 = 1), tornem 0 (=)

    pop {pc}				@; Desapila l'adreça de retorn i salta-hi

@; E9M22_are_unordered_s(): indica si num1 i num2 no són "ordenables",
@;         				    perquè num1 o num2 són NaN (en coma flotant E9M22).
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 i num2 no es poden ordenar.

.global E9M22_are_unordered_s
E9M22_are_unordered_s:
		push {lr}			@; Apila l'adreça de retorn

		mov r0, #0		@; Carrega 0 a R0 per indicar rutina pendent (sempre fals)
	
		pop {pc}			@; Desapila l'adreça de retorn i salta-hi

@; E9M22_is_gt_s(): indica si num1 > num2, considerant valors
@;                	codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 > num2.

.global E9M22_is_gt_s
E9M22_is_gt_s:
	push {lr}						@; Guardem el registre de retorn

	@; Comprovem si num1 o num2 és NaN o infinit
	ldr r2, =E9M22_MASK_EXP			@; Màscara per l'exponent
	tst r0, r2						@; Comprovem si num1 té tots els bits de l'exponent a 1
	bne comprovar_num1_especial			@; Si és NaN o infinit, saltem
	tst r1, r2						@; Comprovem si num2 té tots els bits de l'exponent a 1
	bne check_special_num2			@; Si és NaN o infinit, saltem

comparar_valors:
	@; Comparem si num1 i num2 són zeros (+0.0 o -0.0)
	and r3, r0, #~E9M22_MASK_SIGN	@; Eliminem el bit de signe de num1
	and r4, r1, #~E9M22_MASK_SIGN	@; Eliminem el bit de signe de num2
	cmp r3, #0						@; Comprovem si num1 és zero
	bne no_es_zero					@; Si num1 no és zero, saltem
	cmp r4, #0						@; Comprovem si num2 és zero
	beq es_fals					@; Si tots dos són zeros, retornem fals

no_es_zero:
	@; Comparem operands finits
	cmp r0, r1						@; Comparem num1 amb num2
	bgt es_cert						@; Si num1 > num2, anem a retornar cert
	b es_fals						@; Altrament, anem a retornar fals

comprovar_num1_especial:
	@; Comprovem si num1 és NaN (mantissa ≠ 0)
	ldr r3, =E9M22_MASK_FRAC		@; Màscara per la mantissa
	tst r0, r3						@; Comprovem si num1 té fracció diferent de 0 (és NaN)
	beq comprovar_num1_infinitat			@; Si no és NaN, és infinit

	@; num1 és NaN, retornem fals
	b es_fals						@; Anem a retornar fals

comprovar_num1_infinitat:
	@; Comprovem si num2 és NaN o infinit
	tst r1, r2						@; Comprovem si num2 té tots els bits de l'exponent a 1
	beq num1_es_infinit_num2_es_finit	@; Si num2 és finit, saltem

	@; Comprovem si num2 és NaN (mantissa ≠ 0)
	tst r1, r3						@; Comprovem si num2 té fracció diferent de 0 (és NaN)
	beq comparar_infinits			@; Si no és NaN, és infinit

	@; num2 és NaN, retornem fals
	b es_fals						@; Anem a retornar fals

num1_es_infinit_num2_es_finit:
	@; num1 és infinit, num2 és finit
	and r3, r0, #E9M22_MASK_SIGN		@; Obtenim el signe de num1
	cmp r3, #0						@; Comprovem si num1 és +∞ o -∞
	beq es_cert						@; Si num1 és +∞, num1 > num2
	b es_fals						@; Si num1 és -∞, num1 < num2

comprovar_num2_especial
	@; Comprovem si num2 és NaN (mantissa ≠ 0)
	tst r1, r3						@; Comprovem si num2 té fracció diferent de 0 (és NaN)
	beq comprovar_num2_infinitat			@; Si no és NaN, és infinit

	@; num2 és NaN, retornem fals
	b es_fals						@; Anem a retornar fals

comprovar_num2_infinitat:
	@; num2 és infinit, num1 és finit
	and r3, r1, #E9M22_MASK_SIGN		@; Obtenim el signe de num2
	cmp r3, #0						@; Comprovem si num2 és +∞ o -∞
	beq es_fals					@; Si num2 és +∞, num1 < num2
	b es_cert						@; Si num2 és -∞, num1 > num2

comparar_infinits:
	@; Comparem els infinits
	and r3, r0, #E9M22_MASK_SIGN		@; Signe de num1
	and r4, r1, #E9M22_MASK_SIGN	@; Signe de num2
	cmp r3, r4						@; Comparem els signes
	blt es_fals					@; Si num1 < num2 (signe num1 > signe num2), num1 < num2
	bgt es_cert						@; Si num1 > num2 (signe num1 < signe num2), num1 > num2
	b es_fals						@; Si els signes són iguals, num1 ≤ num2

es_fals:
	mov r0, #0						@; Retornem fals
	b fet_gt						@; Anem al final

es_cert:
	mov r0, #1						@; Retornem cert
	b fet_gt						@; Anem al final

fet_gt:
	pop {pc}						@; Restaurem el registre de retorn

@; E9M22_is_ge_s(): indica si num1 ≥ num2, considerant valors
@;                	codificats en coma flotant E9M22.
@;	Entrada:
@;		num1 	-> R0: primer operand
@;		num2 	-> R1: segon operand
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 ≥ num2.

.global E9M22_is_ge_s
E9M22_is_ge_s:
	push {lr}						@; Guardem el registre de retorn

	@; Comprovem si num1 o num2 és NaN o infinit
	ldr r2, =E9M22_MASK_EXP			@; Màscara per l'exponent
	tst r0, r2						@; Comprovem si num1 té tots els bits de l'exponent a 1
	bne comprovar_num1_especial_ge		@; Si és NaN o infinit, saltem
	tst r1, r2						@; Comprovem si num2 té tots els bits de l'exponent a 1
	bne comprovar_num2_especial_ge		@; Si és NaN o infinit, saltem

comparar_valors_ge:
	@; Comparem si num1 i num2 són zeros (+0.0 o -0.0)
	and r3, r0, #~E9M22_MASK_SIGN	@; Eliminem el bit de signe de num1
	and r4, r1, #~E9M22_MASK_SIGN	@; Eliminem el bit de signe de num2
	cmp r3, #0						@; Comprovem si num1 és zero
	bne no_es_zero_ge					@; Si num1 no és zero, saltem
	cmp r4, #0						@; Comprovem si num2 és zero
	beq es_cert_ge					@; Si tots dos són zeros, retornem cert

no_es_zero_ge:
	@; Comparem operands finits
	cmp r0, r1						@; Comparem num1 amb num2
	bge es_cert_ge					@; Si num1 ≥ num2, anem a retornar cert
	b es_fals_ge					@; Altrament, anem a retornar fals

comprovar_num1_especial_ge:
	@; Comprovem si num1 és NaN (mantissa ≠ 0)
	ldr r3, =E9M22_MASK_FRAC		@; Màscara per la mantissa
	tst r0, r3						@; Comprovem si num1 té fracció diferent de 0 (és NaN)
	beq comprovar_num1_infinitat_ge		@; Si no és NaN, és infinit

	@; num1 és NaN, retornem fals
	b es_fals_ge					@; Anem a retornar fals

comprovar_num1_infinitat_ge:
	@; Comprovem si num2 és NaN o infinit
	tst r1, r2						@; Comprovem si num2 té tots els bits de l'exponent a 1
	beq num1_es_infinit_num2_es_finit_ge @; Si num2 és finit, saltem

	@; Comprovem si num2 és NaN (mantissa ≠ 0)
	tst r1, r3						@; Comprovem si num2 té fracció diferent de 0 (és NaN)
	beq comparar_infinits_ge		@; Si no és NaN, és infinit

	@; num2 és NaN, retornem fals
	b es_fals_ge					@; Anem a retornar fals

num1_es_infinit_num2_es_finit_ge:
	@; num1 és infinit, num2 és finit
	and r3, r0, #E9M22_MASK_SIGN		@; Obtenim el signe de num1
	cmp r3, #0						@; Comprovem si num1 és +∞ o -∞
	beq es_cert_ge					@; Si num1 és +∞, num1 ≥ num2
	b es_fals_ge					@; Si num1 és -∞, num1 < num2

comprovar_num2_especial_ge:
	@; Comprovem si num2 és NaN (mantissa ≠ 0)
	tst r1, r3						@; Comprovem si num2 té fracció diferent de 0 (és NaN)
	beq comprovar_num2_infinitat_ge		@; Si no és NaN, és infinit

	@; num2 és NaN, retornem fals
	b es_fals_ge					@; Anem a retornar fals

comprovar_num2_infinitat_ge:
	@; num2 és infinit, num1 és finit
	and r3, r1, #E9M22_MASK_SIGN		@; Obtenim el signe de num2
	cmp r3, #0						@; Comprovem si num2 és +∞ o -∞
	beq es_fals_ge					@; Si num2 és +∞, num1 < num2
	b es_cert_ge					@; Si num2 és -∞, num1 ≥ num2

comparar_infinits_ge:
	@; Comparem els infinits
	and r3, r0, #E9M22_MASK_SIGN		@; Signe de num1
	and r4, r1, #E9M22_MASK_SIGN	@; Signe de num2
	cmp r3, r4						@; Comparem els signes
	blt es_fals_ge					@; Si num1 < num2 (signe num1 > signe num2), num1 < num2
	bge es_cert_ge					@; Si num1 ≥ num2 (signe num1 ≤ signe num2), num1 ≥ num2

es_fals_ge:
	mov r0, #0						@; Retornem fals
	b end_ge						@; Anem al final

es_cert_ge:
	mov r0, #1						@; Retornem cert
	b end_ge						@; Anem al final

end_ge:
	pop {pc}						@; Restaurem el registre de retorn

@; E9M22_is_lt_s(): indica si num1 < num2, considerant valors
@;                	codificats en coma flotant E9M22.

.global E9M22_is_lt_s
E9M22_is_lt_s:
	push {lr}
	;@ Comprovem si algun operand és NaN
	ldr r2, =E9M22_MASK_EXP
	ldr r3, =E9M22_MASK_FRAC

	tst r0, r2
	bne comprovar_si_num1_es_lt
	tst r1, r2
	bne comprovar_si_num2_es_lt
	b fer_comparacio_amb_lt

comprovar_si_num1_es_lt:
	tst r0, r3
	bne return_false_lt
	tst r1, r2
	beq fer_comparacio_amb_lt
comprovar_si_num2_es_lt:
	tst r1, r3
	bne return_false_lt

fer_comparacio_amb_lt:
	bl E9M22_is_ge_s
	cmp r0, #0
	moveq r0, #1
	movne r0, #0
	pop {pc}

return_false_lt:
	mov r0, #0
	pop {pc}

@; E9M22_is_le_s(): indica si num1 ≤ num2, considerant valors
@;                	codificats en coma flotant E9M22.

.global E9M22_is_le_s
E9M22_is_le_s:
		push {lr}

		mov r0, #0		@; to-do: sempre fals per indicar rutina pendent
	
		pop {pc}

@;**********************************************************
@;* Funcions auxiliars: NORMALITZACIÓ i ARRODONIMENT E9M22 *
@;**********************************************************

@; E9M22_normalize_and_round_s(): normalitza
@;                	i arrodoneix al més proper.
@;	Entrada:
@;		signe 	 -> R0: signe del valor a normalitzar
@;		exponent -> R1: exponent del valor a normalitzar
@;		mantissa -> R2: mantissa del valor a normalitzar
@;	Sortida:
@;		R0 		-> retorna un valor no-zero si num1 ≤ num2.

.global E9M22_normalize_and_round_s
E9M22_normalize_and_round_s:
		push {lr}

		ldr r0, =E9M22_sNAN		@; to-do: NaN per indicar rutina pendent
	
		pop {pc}

@; E9M22_round_s(): Arrodoniment al més proper.
@;                	Si es troba al mig, al més proper parell.
@;	Entrada:
@;		mantissa  -> R0: mantissa del valor a arrodonir
@;		nbits_shr -> R1: quantitat de bits a desplaçar a la dreta
@;	Sortida:
@;		R0 		 -> Retorna la mantissa arrodonida (+1)
@;					o l'original (trunc) sense desplaçar.

.global E9M22_round_s
E9M22_round_s:
		push {lr}

		mov r0, #-1		@; to-do: -1 per indicar rutina pendent
	
		pop {pc}

@;****************************************************************
@;* Funcions AUXILIARS per treballar amb els bits de codificació *
@;****************************************************************

@; count_leading_zeros_s(): compta quants bits a 0 hi ha
@;                			des del bit de més pes (esquerra).
@;	Entrada:
@;		num  -> R0: número de 32 bits a analitzar.
@;	Sortida:
@;		R0 		 -> número de bits a zero (0-32) des del MSB.

.global count_leading_zeros_s
count_leading_zeros_s:
		push {lr}

		mov r0, #123		@; to-do: 123 per indicar rutina pendent
	
		pop {pc}

@; count_trailing_zeros_s(): compta quants bits a 0 hi ha
@;                 			 des del bit de menys pes (dreta).
@;	Entrada:
@;		num  -> R0: número de 32 bits a analitzar.
@;	Sortida:
@;		R0 		 -> número de bits a zero (0-32) des del LSB.

.global count_trailing_zeros_s
count_trailing_zeros_s:
		push {lr}

		mov r0, #123		@; to-do: 123 per indicar rutina pendent
	
		pop {pc}

.end