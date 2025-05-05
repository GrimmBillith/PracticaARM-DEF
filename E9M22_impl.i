@;----------------------------------------------------------------
@;  Selecció implementació (C/ARM) rutines de coma flotant E9M22.
@;	HA DE COINCIDIR amb el seleccionat a E9M22_impl.h.
@;----------------------------------------------------------------
@;	pere.millan@urv.cat
@;	(Març 2025)
@;----------------------------------------------------------------
@;	Programador/a 1: guillem.requeno@estudiants.urv.cat
@;	Programador/a 2: alexxavier.jurado@estudiants.urv.cat
@;----------------------------------------------------------------


@;#########################################################
@; Cal deixar descomentada la versió de rutina a utilitzar
@;#########################################################


@;******************************************************
@;* Rutines de CONVERSIÓ de valors E9M22 <-> float/int *
@;******************************************************

E9M22_to_float	=	E9M22_to_float_c	@; Versió en C
@;E9M22_to_float	=	E9M22_to_float_s	@; Versió assemblador

float_to_E9M22	=	float_to_E9M22_c	@; Versió en C
@;float_to_E9M22	=	float_to_E9M22_s	@; Versió assemblador

E9M22_to_int	=	E9M22_to_int_c	@; Versió en C
@;E9M22_to_int	=	E9M22_to_int_s	@; Versió assemblador

int_to_E9M22	=	int_to_E9M22_c	@; Versió en C
@;int_to_E9M22	=	int_to_E9M22_s	@; Versió assemblador


@;*************************************************
@;* Operacions ARITMÈTIQUES en Coma Flotant E9M22 *
@;*************************************************

E9M22_add	=	E9M22_add_c	@; Versió en C
@;E9M22_add	=	E9M22_add_s	@; Versió assemblador

E9M22_sub	=	E9M22_sub_c	@; Versió en C
@;E9M22_sub	=	E9M22_sub_s	@; Versió assemblador

E9M22_mul	=	E9M22_mul_c	@; Versió en C
@;E9M22_mul	=	E9M22_mul_s	@; Versió assemblador

E9M22_div	=	E9M22_div_c	@; Versió en C
@;E9M22_div	=	E9M22_div_s	@; Versió assemblador

E9M22_neg	=	E9M22_neg_c	@; Versió en C
@;E9M22_neg	=	E9M22_neg_s	@; Versió assemblador

E9M22_abs	=	E9M22_abs_c	@; Versió en C
@;E9M22_abs	=	E9M22_abs_s	@; Versió assemblador


@;*************************************************************
@;* Operacions de COMPARACIÓ de números en Coma Flotant E9M22 *
@;*************************************************************

E9M22_are_eq	=	E9M22_are_eq_c	@; Versió en C
@;E9M22_are_eq	=	E9M22_are_eq_s	@; Versió assemblador

E9M22_are_ne	=	E9M22_are_ne_c	@; Versió en C
@;E9M22_are_ne	=	E9M22_are_ne_s	@; Versió assemblador

E9M22_are_unordered	=		E9M22_are_unordered_c	@; Versió en C
@;E9M22_are_unordered	=	E9M22_are_unordered_s	@; Versió assemblador

E9M22_is_gt	=		E9M22_is_gt_c	@; Versió en C
@;E9M22_is_gt	=	E9M22_is_gt_s	@; Versió assemblador

E9M22_is_ge	=		E9M22_is_ge_c	@; Versió en C
@;E9M22_is_ge	=	E9M22_is_ge_s	@; Versió assemblador

E9M22_is_lt	=		E9M22_is_lt_c	@; Versió en C
@;E9M22_is_lt	=	E9M22_is_lt_s	@; Versió assemblador

E9M22_is_le	=		E9M22_is_le_c	@; Versió en C
@;E9M22_is_le	=	E9M22_is_le_s	@; Versió assemblador


@;**********************************************************
@;* Funcions auxiliars: NORMALITZACIÓ i ARRODONIMENT E9M22 *
@;**********************************************************

E9M22_normalize_and_round	=		E9M22_normalize_and_round_c	@; Versió en C
@;E9M22_normalize_and_round	=	E9M22_normalize_and_round_s	@; Versió assemblador

E9M22_round	=	E9M22_round_c	@; Versió en C
@;E9M22_round	=	E9M22_round_s	@; Versió assemblador


@;****************************************************************@;
@;* Funcions AUXILIARS per treballar amb els bits de codificació *@;
@;****************************************************************@;

count_leading_zeros	=	count_leading_zeros_c	@; Versió en C
@;count_leading_zeros	=	count_leading_zeros_s	@; Versió assemblador

count_trailing_zeros	=	count_trailing_zeros_c	@; Versió en C
@;count_trailing_zeros	=	count_trailing_zeros_s	@; Versió assemblador



