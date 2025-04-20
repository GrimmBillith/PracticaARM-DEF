@;-----------------------------------------------------------------------
@;  Description: a program to check the temperature-scale conversion
@;   			 functions implemented in "CelsiusFahrenheit_c/_s".
@;    IMPORTANT NOTE: there is a much confident testing set
@;   			 implemented in "tests/test_CelsiusFahrenheit.c";
@;   			 the aim of "demo_CelsFahr.s" is to show how would it be
@;   			 a usual main() code invoking the mentioned functions.
@;-----------------------------------------------------------------------
@;    Author: Santiago Romaní, Pere Millán (DEIM, URV)
@;    Date:   March/2022, March/2023, February/2024, March/2025
@;-----------------------------------------------------------------------
@;    Programmer 1: guillem.requeno@estudiants.urv.cat
@;    Programmer 2: alexxavier.jurado@estudiants.urv.cat
@;-----------------------------------------------------------------------*/

.data
    	.align 2
	temp1C:    .word 0x41066B85   	 @; temp1C =  35.21 °C
	temp2F:    .word 0xC0DF0000   	 @; temp2F = -23.75 °F

.bss
    	.align 2
	temp1F:    .space 4   			 @; expected conversion:  95.3780 °F
	temp2C:    .space 4   			 @; expected conversion: -30.9722 °C


.text
    	.align 2
    	.arm
    	.global main
main:
    	push {lr}
        	 
   	 @; temp1F = Celsius2Fahrenheit(temp1C);
    	@; Convertim temp1C (°C) a Fahrenheit (°F)
    	ldr r0, =temp1C   			 @; Carreguem l'adreça de temp1C a r0
    	ldr r0, [r0]   			 @; Carreguem el valor de temp1C a r0
    	bl Celsius2Fahrenheit   	 @; Cridem la funció Celsius2Fahrenheit
    	ldr r1, =temp1F   			 @; Carreguem l'adreça de temp1F a r1
    	str r0, [r1]   			 @; Desem el resultat (°F) a temp1F
    
   	 @; temp2C = Fahrenheit2Celsius(temp2F);
    	@; Convertim temp2F (°F) a Celsius (°C)
    	ldr r0, =temp2F   			 @; Carreguem l'adreça de temp2F a r0
    	ldr r0, [r0]   			 @; Carreguem el valor de temp2F a r0
    	bl Fahrenheit2Celsius   	 @; Cridem la funció Fahrenheit2Celsius
    	ldr r1, =temp2C   			 @; Carreguem l'adreça de temp2C a r1
    	str r0, [r1]   			 @; Desem el resultat (°C) a temp2C
   	 

@; TESTING POINT: check the results
@;    (gdb) p /x temp1F   	 -> 0x415F60C4
@;    (gdb) p /x temp2C   	 -> 0xC0FBE38E
@; BREAKPOINT
    	mov r0, #0   				 @; return(0)
   	 
    	pop {pc}

.end

