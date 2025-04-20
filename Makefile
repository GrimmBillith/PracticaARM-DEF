
#-------------------------------------------------------------------------------
# Example Makefile to build a library and to test the functions of each module
# Authors: Santiago Romaní, Pere Millán
# Date: April 2021, March 2022-2024, February-March 2025
#-------------------------------------------------------------------------------
#	Programador/a 1: guillem.requeno@estudiants.urv.cat
#	Programador/a 2: alexxavier.jurado@estudiants.urv.cat
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# options for code generation
#-------------------------------------------------------------------------------
ARCH	:= -march=armv5te -mlittle-endian
INCL    := -I./include
ASFLAGS	:= $(ARCH) $(INCL) -g
CCFLAGS	:= -Wall -gdwarf-3 -O0 $(ARCH) $(INCL)
LDFLAGS := -z max-page-size=0x8000 

#-------------------------------------------------------------------------------
# make commands
#-------------------------------------------------------------------------------
tota_1a_part_practica_FC : libE9M22.a demo_pi.elf demo_CelsFahr.elf demo_E9M22.elf test_CelsiusFahrenheit.elf test_E9M22.elf 
    #$(warning Recorda: aquest Makefile esta incomplet. Completa les regles faltants!! ... i esborra aquest warning.)
	@echo "Compilació completada."

build/demo_pi.o: source/demo_pi.s include/E9M22.i include/E9M22_impl.i
	arm-none-eabi-as $(ASFLAGS) -c source/demo_pi.s -o build/demo_pi.o

demo_pi.elf : build/demo_pi.o libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/demo_pi.o p_lib/startup.o \
					libE9M22.a p_lib/libfoncompus.a -o demo_pi.elf


build/demo_E9M22.o : source/demo_E9M22.c include/E9M22.h include/E9M22_impl.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/demo_E9M22.c -o build/demo_E9M22.o

demo_E9M22.elf : build/demo_E9M22.o libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/demo_E9M22.o p_lib/startup.o \
					libE9M22.a p_lib/libfoncompus.a -o demo_E9M22.elf


build/demo_CelsFahr.o: source/demo_CelsFahr.s include/E9M22.i include/E9M22_impl.i
	arm-none-eabi-as $(ASFLAGS) -c source/demo_CelsFahr.s -o build/demo_CelsFahr.o
    

demo_CelsFahr.elf : build/demo_CelsFahr.o build/CelsiusFahrenheit_s.o libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/demo_CelsFahr.o build/CelsiusFahrenheit_s.o p_lib/startup.o \
                    libE9M22.a p_lib/libfoncompus.a -o demo_CelsFahr.elf
    


build/CelsiusFahrenheit_c.o : source/CelsiusFahrenheit_c.c include/E9M22.h include/E9M22_impl.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/CelsiusFahrenheit_c.c -o build/CelsiusFahrenheit_c.o

build/CelsiusFahrenheit_s.o: source/CelsiusFahrenheit_s.s include/E9M22.i include/E9M22_impl.i
	arm-none-eabi-as $(ASFLAGS) -c source/CelsiusFahrenheit_s.s -o build/CelsiusFahrenheit_s.o


#-------------------------------------------------------------------------------
# libE9M22 making commands
#-------------------------------------------------------------------------------
libE9M22.a: build/E9M22_c.o build/E9M22_s.o build/E9M22_aux.o 
	arm-none-eabi-ar -rs libE9M22.a build/E9M22_s.o build/E9M22_c.o build/E9M22_aux.o 

build/E9M22_c.o : source/E9M22_c.c include/E9M22.h include/E9M22_impl.h 
	arm-none-eabi-gcc $(CCFLAGS) -c source/E9M22_c.c -o build/E9M22_c.o

build/E9M22_s.o: source/E9M22_s.s include/E9M22.i include/E9M22_impl.i
	arm-none-eabi-as $(ASFLAGS) -c source/E9M22_s.s -o build/E9M22_s.o

build/E9M22_aux.o: source/E9M22_aux.s 
	arm-none-eabi-as $(ASFLAGS) -c source/E9M22_aux.s -o build/E9M22_aux.o


#-------------------------------------------------------------------------------
# test making commands
#-------------------------------------------------------------------------------
test_CelsiusFahrenheit.elf : build/test_CelsiusFahrenheit.o build/CelsiusFahrenheit_c.o libE9M22.a 
	arm-none-eabi-ld $(LDFLAGS) build/test_CelsiusFahrenheit.o build/CelsiusFahrenheit_c.o p_lib/startup.o \
					libE9M22.a p_lib/libfoncompus.a -o test_CelsiusFahrenheit.elf

build/test_CelsiusFahrenheit.o : tests/test_CelsiusFahrenheit.c include/E9M22.h include/CelsiusFahrenheit.h 
	arm-none-eabi-gcc $(CCFLAGS) -c tests/test_CelsiusFahrenheit.c -o build/test_CelsiusFahrenheit.o


test_E9M22.elf : build/test_E9M22.o libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/test_E9M22.o p_lib/startup.o \
					libE9M22.a p_lib/libfoncompus.a -o test_E9M22.elf
    # Regla completada para generar el ejecutable de prueba test_E9M22.elf


build/test_E9M22.o : tests/test_E9M22.c include/E9M22_impl.h
	arm-none-eabi-gcc $(CCFLAGS) -c tests/test_E9M22.c -o build/test_E9M22.o
    # Regla completada para compilar el archivo .c en un objeto .o



#-------------------------------------------------------------------------------
# clean commands
#-------------------------------------------------------------------------------
clean : 
	@rm -fv build/*
	@rm -fv *.elf
	@rm -fv libE9M22.a


#-----------------------------------------------------------------------------
# run commands
#-----------------------------------------------------------------------------
run : demo_pi.elf	demo_CelsFahr.elf	demo_E9M22.elf
	arm-eabi-insight demo_pi.elf &
    # arm-eabi-insight demo_CelsFahr.elf &
    # arm-eabi-insight demo_E9M22.elf &


#-----------------------------------------------------------------------------
# debug commands
#-----------------------------------------------------------------------------
debug : test_CelsiusFahrenheit.elf	test_E9M22.elf 
    # arm-eabi-insight test_CelsiusFahrenheit.elf &
    # arm-eabi-insight test_E9M22.elf &

