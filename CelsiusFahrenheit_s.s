@;----------------------------------------------------------------
@;  CelsiusFahrenheit.s: rutines de conversió de temperatura
@;   					  en format de Coma Flotant E9M22.
@;   	 Implementació cridant rutines aritmètiques libE9M22.
@;----------------------------------------------------------------
@;    santiago.romani@urv.cat
@;    pere.millan@urv.cat
@;    (Març 2021-2023, Febrer 2024, Març 2025)
@;----------------------------------------------------------------
@;    Programador/a 1: guillem.requeno@estudiants.urv.cat
@;    Programador/a 2: alexxavier.jurado@estudiants.urv.cat
@;----------------------------------------------------------------*/

.include "E9M22.i"    @; operacions en coma flotant E9M22
@; Constants en format E9M22
.data
	.align 2
E9M22_9_div_5:    .word 0x3FFCCCCC@; Valor de 9/5 en format E9M22
E9M22_32:   	 .word 0x40200000@; Valor de 32.0 en format E9M22
E9M22_5_div_9:    .word 0x3FE71C72@; Valor de 5/9 en format E9M22
.text
    	.align 2
    	.arm


@; Celsius2Fahrenheit(): converteix una temperatura en graus Celsius
@;   					 a la temperatura equivalent en graus Fahrenheit,
@;   					 usant valors codificats en Coma Flotant E9M22.
@;    Entrada:
@;   	 input     -> R0
@;    Sortida:
@;   	 R0    	 -> output = (input * 9/5) + 32.0;
	.global Celsius2Fahrenheit
Celsius2Fahrenheit:
    	push {r4, lr}   		 @; Guardem registres que utilitzarem

    	@; Carreguem constants
    	ldr r1, =E9M22_9_div_5    @; Constant 9/5 en format E9M22
    	ldr r2, =E9M22_32   	 @; Constant 32.0 en format E9M22

    	@; Part 1: resultat = input * 9/5
    	bl E9M22_mul   		 @; Multiplica input (R0) per 9/5 (R1)
    	mov r4, r0   			 @; Guardem el resultat temporal a R4

    	@; Part 2: resultat = (input * 9/5) + 32.0
    	mov r0, r4   			 @; Movem el resultat anterior a R0
    	mov r1, r2   			 @; Movem la constant 32.0 a R1
    	bl E9M22_add_c   		 @; Sumem els dos valors

    	pop {r4, pc}   		 @; Restaurem registres i retornem


@; Fahrenheit2Celsius(): converteix una temperatura en graus Fahrenheit
@;   					 a la temperatura equivalent en graus Celsius,
@;   					 usant valors codificats en Coma Flotant E9M22.
@;    Entrada:
@;   	 input     -> R0
@;    Sortida:
@;   	 R0    	 -> output = (input - 32.0) * 5/9;
	.global Fahrenheit2Celsius
Fahrenheit2Celsius:
    	push {r4, lr}   		 @; Guardem registres que utilitzarem

    	@; Carreguem constants
    	ldr r1, =E9M22_32   	 @; Constant 32.0 en format E9M22
    	ldr r2, =E9M22_5_div_9    @; Constant 5/9 en format E9M22

    	@; Part 1: resultat = input - 32.0
    	bl E9M22_sub   		 @; Restem input (R0) menys 32.0 (R1)
    	mov r4, r0   			 @; Guardem el resultat temporal a R4

    	@; Part 2: resultat = (input - 32.0) * 5/9
    	mov r0, r4   			 @; Movem el resultat anterior a R0
    	mov r1, r2   			 @; Movem la constant 5/9 a R1
    	bl E9M22_mul   		 @; Multipliquem els dos valors

    	pop {r4, pc}   		 @; Restaurem registres i retornem




.end