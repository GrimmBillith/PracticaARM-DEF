@;-----------------------------------------------------------------------
@;  Fitxer:		demo_pi.s
@;  Descripció: exemple de crida a rutina E9M22 i accés a vars. globals.
@;-----------------------------------------------------------------------
@;	Autor: Pere Millán (DEIM, URV)
@;	Data:  Març/2025 
@;-----------------------------------------------------------------------

.include "E9M22.i"

.text
        .align 2
        .arm


.data	@; Variables globals inicialitzades:
		.align 2
	pi_float:	.float 3.141592654		@; Valor de π amb prou decimals

					@; Altres valors float d'exemple:
	reals32bits: .float 543.21, -12.345, 1E-43, -infinity, +infinity, -nan, -0.0 


.bss	@; Variables globals no inicialitzades:
		.align 2
	pi_E9M22:	.space 4	@; Per desar π en format E9M22
	pi_float2:	.space 4	@; Valor inicial de π en float? 


.text
		.align 2
		.arm
		.global main
main:
		push {r1-r3, lr}

	@;-------------------------------------------------------------------	
	@; Ús de REGISTRES:
	@;  R0: adreces de variables globals + paràmetre/resultat de rutines
	@;	R1: valor de pi_float (format IEEE754 binary32)
	@;  R2: valor de pi_E9M22 (format coma flotant E9M22)
	@;	R3: valor de pi_float2 (format IEEE754 binary32)
	@;-------------------------------------------------------------------	

			@; pi_E9M22 = float_to_E9M22(pi_float);

		ldr r0, =pi_float	@; R0: adreça de la variable global pi_float
		ldr r1, [r0]		@; R1: valor de pi_float

		mov r0, r1			@; preparar paràmetre rutina float_to_E9M22
		bl float_to_E9M22	@; cridar a float_to_E9M22 (resultat a R0)
		mov r2, r0			@; R2: conservar valor π convertit a format E9M22

		ldr r0, =pi_E9M22	@; R0: adreça de la variable global pi_E9M22
		str r2, [r0]		@; desar valor π en format E9M22 a var. global pi_E9M22

@; TESTING POINT: check the data (at registers: r1, r2)
@;	(gdb) p /f $r1		-> 3.14159274 
@;	(gdb) p /x $r2		-> 0x402487ed 
@; BREAKPOINT

	@;-------------------------------------------------------------------	

			@; pi_float2 = E9M22_to_float(pi_E9M22);

		mov r0, r2			@; preparar paràmetre rutina E9M22_to_float
		bl E9M22_to_float	@; cridar a E9M22_to_float (resultat a R0)
		mov r3, r0			@; R3: conservar valor π convertit de nou a float

		ldr r0, =pi_float2	@; R0: adreça de la variable global pi_float2
		str r3, [r0]		@; desar valor π en format float a var. global pi_float2


@; TESTING POINT: check the results (at memory: global vars)
@;	(gdb) p /f pi_float		-> 3.14159274 
@;	(gdb) p /x pi_E9M22		-> 0x402487ed 
@;	(gdb) p /f pi_float2	-> 3.1415925 
@; BREAKPOINT

		mov r0, #0			@; return(0)
		
		pop {r1-r3, pc}

.end

