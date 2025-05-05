@;-----------------------------------------------------------------------
@;  Description: a program to check the temperature-scale conversion
@;				functions implemented in "CelsiusFahrenheit.c".
@;	IMPORTANT NOTE: there is a much confident testing set implemented in
@;				"tests/test_CelsiusFahrenheit.c"; the aim of "demo.s" is
@;				to show how would it be a usual main() code invoking the
@;				mentioned functions.
@;-----------------------------------------------------------------------
@;	Author: Santiago Romaní, Pere Millán (DEIM, URV)
@;	Date:   March/2022-2023, February/2024, March/2025 
@;-----------------------------------------------------------------------
@;	Programmer 1: guillem.requeno@estudiants.urv.cat
@;	Programmer 2: alexxavier.jurado@estudiants.urv.cat
@;-----------------------------------------------------------------------*/

.include "E9M22.i"			@; operacions en coma flotant E9M22

.include "avgmaxmintemp.i"	@; offsets per accedir a structs 'maxmin_t'


.text
        .align 2
        .arm


@; avgmaxmin_city(): calcula la temperatura mitjana, màxima i mínima 
@;				d'una ciutat d'una taula de temperatures, 
@;				amb una fila per ciutat i una columna per mes, 
@;				expressades en graus Celsius en format E9M22.
@;	Entrada:
@;		ttemp[][12]	→	R0: taula de temperatures, amb 12 columnes i nrows files
@;		nrows		→	R1: número de files de la taula
@;		id_city		→	R2: índex de la fila (ciutat) a processar
@;		*mmres		→	R3: adreça de l'estructura maxmin_t que retornarà els
@;						resultats de temperatures màximes i mínimes
@;	Sortida:
@;		R0			→	temperatura mitjana, expressada en graus Celsius, en format E9M22.		

.data
.align 4
E9M22_12: .word 0x40A00000    @; 12.0 en E9M22

.global avgmaxmin_city
avgmaxmin_city:
        push {r4-r12, lr}

        mov r4, r3              @; mmres

        mov r8, #12
        mul r8, r2, r8
        lsl r8, r8, #2
        add r8, r0, r8          @; r8 = &ttemp[id_city][0]

        mov r5, #0              @; i = 0
        mov r9, #0              @; suma = 0 (E9M22 zero)
        ldr r12, [r8]           @; primer valor

        mov r10, r12            @; max = ttemp[0]
        mov r11, r12            @; min = ttemp[0]
        mov r6, #0              @; idmin = 0
        mov r7, #0              @; idmax = 0

loop:
        cmp r5, #12
        bge end_loop

        ldr r12, [r8, r5, lsl #2] @; t = ttemp[i]

        @; acumulación
        mov r0, r9
        mov r1, r12
        bl E9M22_add
        mov r9, r0              @; suma += t

        @; máximo (solo si t > max → primer gana)
        mov r0, r12
        mov r1, r10
        bl E9M22_is_gt
        cmp r0, #0
        beq check_mini
        mov r10, r12
        mov r7, r5              @; Actualiza índice solo si es estrictamente mayor

check_mini:
        @; mínimo (solo si t < min → primer gana)
        mov r0, r12
        mov r1, r11
        bl E9M22_is_lt
        cmp r0, #0
        beq next
        mov r11, r12
        mov r6, r5              @; Actualiza índice solo si es estrictamente menor

next:
        add r5, r5, #1
        b loop

end_loop:
        ldr r1, =E9M22_12
        ldr r1, [r1]
        mov r0, r9
        bl E9M22_div
        mov r9, r0 				@; mitjana

        @; guardar resultados
        str r11, [r4, #0]       @; tmin_C
        str r10, [r4, #4]       @; tmax_C

        mov r0, r11
        bl Celsius2Fahrenheit
        str r0, [r4, #8]

        mov r0, r10
        bl Celsius2Fahrenheit
        str r0, [r4, #12]

        strh r6, [r4, #16]
        strh r7, [r4, #18]

        mov r0, r9              @; retorna la mitjana

        pop {r4-r12, pc}


	
@; avgmaxmin_month(): calcula la temperatura mitjana, màxima i mínima 
@;				d'un mes d'una taula de temperatures, 
@;				amb una fila per ciutat i una columna per mes, 
@;				expressades en graus Celsius en format E9M22.
@;	Entrada:
@;		ttemp[][12]	→	R0: taula de temperatures, amb 12 columnes i nrows files
@;		nrows		→	R1: número de files de la taula
@;		id_month	→	R2: índex de la columna (mes) a processar
@;		*mmres		→	R3: adreça de l'estructura maxmin_t que retornarà els
@;						resultats de temperatures màximes i mínimes
@;	Sortida:
@;		R0			→	temperatura mitjana, expressada en graus Celsius, en format E9M22.		



.global avgmaxmin_month
avgmaxmin_month:
    push {r4-r12, lr}        @; Desa els registres (r4-r12) i l'adreça de retorn (lr) a la pila.

    mov r4, r3              @; r4 = mmres (adreça de l'estructura maxmin_t)
    mov r5, r1              @; r5 = nrows (nombre de files de la taula)
    mov r6, r2              @; r6 = id_month (identificador del mes que volem processar)
    mov r7, r0              @; r7 = ttemp (adreça base de la taula de temperatures)

    @; Calcula l'offset del mes (un cop, abans del bucle)
    lsl r6, #2              @; r6 = id_month * 4 (desplaçament en bytes). Cada temperatura ocupa 4 bytes.

    @; Inicialitza els valors amb la primera fila de la taula, per al mes especificat.
    add r9, r7, r6          @; r9 = &ttemp[0][id_month] (adreça de la primera temperatura del mes)
    ldr r10, [r9]           @; r10 = avg (temperatura mitjana inicial)
    mov r11, r10            @; r11 = max (temperatura màxima inicial)
    mov r12, r10            @; r12 = min (temperatura mínima inicial)
    mov r3, #0              @; r3 = idmax (índex de la fila amb la temperatura màxima inicial)
    mov r2, #0              @; r2 = idmin (índex de la fila amb la temperatura mínima inicial)
    mov r8, #1              @; r8 = i = 1 (comença el bucle des de la segona fila)

loop_month:
    cmp r8, r5              @; i < nrows ? (Compara l'índex de la fila amb el nombre total de files)
    bge end_loop_month      @; Si i >= nrows, salta a end_loop_month (fi del bucle)

    @; Calcula l'adreça de ttemp[i][id_month] *dins* del bucle, de manera explícita.
    mov r9, r8              @; r9 = i (índex de la fila actual)
    mov r0, #48             @; r0 = 48 (bytes per fila: 12 columnes * 4 bytes/temperatura)
    mul r9, r0              @; r9 = i * bytes_per_row (desplaçament de la fila actual)
    add r9, r9, r6          @; r9 = row_offset + month_offset (desplaçament total fins a la columna del mes)
    add r9, r9, r7          @; r9 = base_address + row_offset + month_offset
                                @; r9 ara conté l'adreça de memòria de ttemp[i][id_month]

    ldr r0, [r9]            @; r0 = temperatura actual (tvar)
    mov r1, r0              @; r1 = tvar (copia la temperatura actual a un registre temporal)

    @; 1. Acumula la suma per a calcular la mitjana
    mov r0, r10             @; r0 = avg (temperatura mitjana actual)
    bl E9M22_add            @; r0 = E9M2M_add(avg, tvar) (crida a la funció per a sumar en format E9M22)
    mov r10, r0             @; avg = r0 (actualitza la mitjana)

    @; 2. Comprova si la temperatura actual és més gran que la màxima actual
    mov r1, r11
	ldr r0, [r9]             @; r0 = max (temperatura màxima actual)
    bl E9M22_is_gt          @; r0 = E9M22_is_gt(r1, max) (crida a la funció de comparació amb tvar)
    cmp r0, #0
    beq check_min 
	ldr r0, [r9] @; Si r0 <= 1 (tvar <= max), salta a check_min
    mov r11, r0             @; max = tvar (actualitza la màxima amb la temperatura actual)
    mov r3, r8              @; idmax = i (actualitza l'índex de la fila amb la màxima)

check_min:
    @; 3. Comprova si la temperatura actual és més petita que la mínima actual
    mov r1, r12
	ldr r0, [r9]	@; r0 = min (temperatura mínima actual)
    bl E9M22_is_lt          @; r0 = E9M22_is_lt(r1, min) (crida a la funció de comparació amb tvar)
    cmp r0, #0
    beq next_i
	ldr r0, [r9]	@; Si r0 >= 1 (tvar >= min), salta a next_i
    mov r12, r0             @; min = tvar (actualitza la mínima amb la temperatura actual)
    mov r2, r8              @; idmin = i (actualitza l'índex de la fila amb la mínima)

next_i:
    add r8, r8, #1          @; i++ (incrementa l'índex de la fila)
    b loop_month            @; Torna a l'inici del bucle

end_loop_month:
    @; Calcula la temperatura mitjana (suma / nombre de files)
    mov r0, r5              @; r0 = nrows (nombre de files)
    bl int_to_E9M22         @; r0 = int_to_E9M22(nrows) (converteix el nombre de files a format E9M22)
    mov r1, r0              @; r1 = divisor (divisor per a la divisió, nrows en format E9M22)
    mov r0, r10             @; r0 = avg (suma total de les temperatures)
    bl E9M22_div            @; r0 = E9M22_div(avg, nrows_en_E9M22) (divideix la suma pel nombre de files en format E9M22)
    mov r10, r0             @; avg = r0 (resultat de la divisió, la mitjana)

    @; 4. Desa els resultats a l'estructura maxmin_t
    str r12, [r4, #0]        @; mmres->tmin_C = min (temperatura mínima en Celsius)
    str r11, [r4, #4]        @; mmres->tmax_C = max (temperatura màxima en Celsius)
    strh r2, [r4, #16]       @; mmres->id_min = idmin (índex de la fila amb la temperatura mínima)
    strh r3, [r4, #18]       @; mmres->id_max = idmax (índex de la fila amb la temperatura màxima)

    @; 5. Converteix les temperatures mínimes i màximes de Celsius a Fahrenheit
    mov r0, r12             @; r0 = min (temperatura mínima en Celsius)
    bl Celsius2Fahrenheit    @; r0 = Celsius2Fahrenheit(min) (converteix a Fahrenheit)
    str r0, [r4, #8]        @; mmres->tmin_F = r0 (temperatura mínima en Fahrenheit)

    mov r0, r11             @; r0 = max (temperatura màxima en Celsius)
    bl Celsius2Fahrenheit    @; r0 = Celsius2Fahrenheit(max) (converteix a Fahrenheit)
    str r0, [r4, #12]       @; mmres->tmax_F = r0 (temperatura màxima en Fahrenheit)

    mov r0, r10             @; r0 = avg (temperatura mitjana en Celsius)
    pop {r4-r12, pc}        @; Restaura els registres i torna de la funció.
                                @; pc = adreça de retorn (del registre lr)

.end

