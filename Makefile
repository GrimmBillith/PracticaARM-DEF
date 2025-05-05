#-------------------------------------------------------------------------------
# Example Makefile to build a library and to test the functions of each module
# Authors: Santiago Romaní, Pere Millán
# Date: April 2021, March 2022-2024, March 2025
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
tota_2a_part_practica_FC : test_geotemp_c.elf test_geotemp_s.elf geotemp_c.elf geotemp_s.elf

build/data.o : source/data.c include/E9M22.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/data.c -o build/data.o

build/geotemp.o : source/geotemp.c include/E9M22.h  include/avgmaxmintemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/geotemp.c -o build/geotemp.o
	
build/avgmaxmintemp_c.o : source/avgmaxmintemp_c.c include/avgmaxmintemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/avgmaxmintemp_c.c -o build/avgmaxmintemp_c.o

build/avgmaxmintemp_s.o : source/avgmaxmintemp_s.s include/avgmaxmintemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c source/avgmaxmintemp_s.s -o build/avgmaxmintemp_s.o

geotemp_c.elf : build/data.o build/geotemp.o build/avgmaxmintemp_c.o p_lib/CelsiusFahrenheit_c.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/geotemp.o build/avgmaxmintemp_c.o build/data.o \
                     p_lib/CelsiusFahrenheit_c.o p_lib/startup.o p_lib/libE9M22.a \
                      p_lib/libfoncompus.a -o geotemp_c.elf


geotemp_s.elf : build/data.o build/geotemp.o build/avgmaxmintemp_s.o p_lib/CelsiusFahrenheit_c.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/geotemp.o build/avgmaxmintemp_s.o build/data.o \
                     p_lib/CelsiusFahrenheit_c.o p_lib/startup.o p_lib/libE9M22.a \
                      p_lib/libfoncompus.a -o geotemp_s.elf

#-------------------------------------------------------------------------------
# test making commands
#-------------------------------------------------------------------------------
build/test_geotemp.o : tests/test_geotemp.c include/E9M22.h include/avgmaxmintemp.h
	arm-none-eabi-gcc $(CCFLAGS) -c tests/test_geotemp.c -o build/test_geotemp.o

test_geotemp_c.elf : build/test_geotemp.o build/avgmaxmintemp_c.o p_lib/CelsiusFahrenheit_c.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/test_geotemp.o build/avgmaxmintemp_c.o \
					 p_lib/CelsiusFahrenheit_c.o p_lib/startup.o p_lib/libE9M22.a \
					 p_lib/libfoncompus.a -o test_geotemp_c.elf

test_geotemp_s.elf : build/test_geotemp.o build/avgmaxmintemp_s.o p_lib/CelsiusFahrenheit_c.o p_lib/libE9M22.a
	arm-none-eabi-ld $(LDFLAGS) build/test_geotemp.o build/avgmaxmintemp_s.o \
					 p_lib/CelsiusFahrenheit_c.o p_lib/startup.o p_lib/libE9M22.a \
					 p_lib/libfoncompus.a -o test_geotemp_s.elf


#-------------------------------------------------------------------------------
# clean commands
#-------------------------------------------------------------------------------
clean : 
	@rm -fv build/*
	@rm -fv *.elf


#-----------------------------------------------------------------------------
# run commands
#-----------------------------------------------------------------------------
run : geotemp_c.elf geotemp_s.elf
	#arm-eabi-insight geotemp_c.elf &
	arm-eabi-insight geotemp_s.elf &


#-----------------------------------------------------------------------------
# debug commands
#-----------------------------------------------------------------------------
debug : test_geotemp_c.elf test_geotemp_s.elf
	#arm-eabi-insight test_geotemp_c.elf &
	arm-eabi-insight test_geotemp_s.elf &