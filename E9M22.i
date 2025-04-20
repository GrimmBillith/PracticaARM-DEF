@;----------------------------------------------------------------
@;  Declaració de constants GAS per al tractament de dades 
@;	en format Coma Flotant E9M22.
@;----------------------------------------------------------------
@;	pere.millan@urv.cat
@;	(Febrer-Març 2025)
@;----------------------------------------------------------------

.include "E9M22_impl.i"

    @; Número de bits i excés/bias del tipus coma flotant E9M22:
E9M22_e		=	9						@; quantitat de bits d'exponent: 9
E9M22_m		= (31-E9M22_e)				@; quantitat de bits de fracció/mantissa/significand: 22 
E9M22_bias	= ((1<<( E9M22_e - 1))-1)	@; bias/excés: 255 


    @; Exponents màxim i mínim del tipus coma flotant E9M22:
E9M22_Emax	=	E9M22_bias
E9M22_Emin	=	(1-E9M22_bias)


    @; MÀSCARES per als camps de bits de valors E9M22:
E9M22_MASK_FRAC	= ((1<<E9M22_m)-1)		@; bits 21..0:	fracció/mantissa/significand 0x003FFFFF
E9M22_MASK_EXP	= (0x7FFFFFFF ^ E9M22_MASK_FRAC)	
                                        @; bits 30..22:	exponent (en excés 255) 
E9M22_MASK_SIGN	= (1<<31)				@; bit 31:		signe 0x80000000 
E9M22_1_IMPLICIT_NORMAL = (1<<E9M22_m)	@; 1 implícit als valors normals 


    @; Alguns valors en coma flotant E9M22:
E9M22_ZERO_POS	=	0			@; +0,0 en coma flotant E9M22 
E9M22_ZERO_NEG	=	0x80000000	@; -0,0 en coma flotant E9M22 
E9M22_INF_POS	=	0x7FC00000	@; +Infinit en coma flotant E9M22
E9M22_INF_NEG	=	0xFFC00000	@; -Infinit en coma flotant E9M22 
E9M22_qNAN		=	0x7FE00000	@; Un possible NaN (quiet) en coma flotant E9M22 
E9M22_sNAN		=	0x7FD00000	@; Un possible NaN (signaling) en coma flotant E9M22 

E9M22_MAX_NORM	=	0x7FBFFFFF	@; Valor màxim normalitzat (positiu)
E9M22_MIN_NORM	=	0x004FFFFF	@; Valor mínim normalitzat (positiu)
E9M22_MAX_DNORM	=	0x003FFFFF	@; Valor màxim denormal (positiu)
E9M22_MIN_DNORM	=	0x00000001	@; Valor mínim denormal (positiu)

E9M22_1			=	0x3FC00000	@; Valor +1,0 en format E9M22
E9M22_2			=	0x40000000	@; Valor +2,0 en format E9M22
E9M22_5			=	0x40500000	@; Valor +5,0 en format E9M22
E9M22_10		=	0x40900000	@; Valor +10,0 en format E9M22
E9M22_20		=	0x40D00000	@; Valor +20,0 en format E9M22
E9M22_50		=	0x41240000	@; Valor +50,0 en format E9M22
E9M22_100		=	0x41640000	@; Valor +100,0 en format E9M22
E9M22_200		=	0x41A40000	@; Valor +200,0 en format E9M22
E9M22_500		=	0x41FD0000	@; Valor +500,0 en format E9M22

E9M22_1000		=	0x423D0000	@; Valor +1.000,0 en format E9M22
E9M22_2000		=	0x427D0000	@; Valor +2.000,0 en format E9M22
E9M22_5000		=	0x42CE2000	@; Valor +5.000,0 en format E9M22
E9M22_10_000	=	0x430E2000	@; Valor +10.000,0 en format E9M22
E9M22_20_000	=	0x434E2000	@; Valor +20.000,0 en format E9M22
E9M22_50_000	=	0x43A1A800	@; Valor +50.000,0 en format E9M22
E9M22_100_000	=	0x43E1A800	@; Valor +100.000,0 en format E9M22
E9M22_200_000	=	0x4421A800	@; Valor +200.000,0 en format E9M22
E9M22_500_000	=	0x447A1200	@; Valor +500.000,0 en format E9M22

E9M22_1_000_000	=	0x44BA1200	@; Valor +1.000.000,0 en format E9M22
E9M22_2_000_000	=	0x44FA1200	@; Valor +2.000.000,0 en format E9M22
E9M22_5_000_000	=	0x454C4B40	@; Valor +5.000.000,0 en format E9M22

E9M22_0_1		=	0x3EE66666	@; Aprox. +0,1 en format E9M22
E9M22_0_2		=	0x3F266666	@; Aprox. +0,2 en format E9M22
E9M22_0_5		=	0x3F800000	@; Valor +0,5 en format E9M22
E9M22_0_01		=	0x3E11EB85	@; Aprox. +0,01 en format E9M22
E9M22_0_02		=	0x3E51EB85	@; Aprox. +0,02 en format E9M22
E9M22_0_05		=	0x3EA66666	@; Aprox. +0,05 en format E9M22
E9M22_0_001		=	0x3D418937	@; Aprox. +0,001 en format E9M22
E9M22_0_002		=	0x3D818937	@; Aprox. +0,002 en format E9M22
E9M22_0_005		=	0x3DD1EB85	@; Aprox. +0,005 en format E9M22
E9M22_0_0001	=	0x3C68DB8B	@; Aprox. +0,0001 en format E9M22
E9M22_0_0002	=	0x3CA8DB8B	@; Aprox. +0,0002 en format E9M22
E9M22_0_0005	=	0x3D018937	@; Aprox. +0,0005 en format E9M22


