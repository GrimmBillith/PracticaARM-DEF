@;-----------------------------------------------------------------------
@;  Fitxer:		E9M22_aux.s
@;  Descripció: rutines auxiliars per a rutines en Coma Flotant E9M22.
@;-----------------------------------------------------------------------
@;	Autor: Pere Millán (DEIM, URV)
@;	Data:  Març/2025 
@;-----------------------------------------------------------------------*/


.text
        .align 2
        .arm


@;******************************************************/
@;*  Rutines per desar regs L/M modificats per codi C  */
@;******************************************************/

		.global E9M22_to_float_c
E9M22_to_float_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_to_float_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global float_to_E9M22_c
float_to_E9M22_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   float_to_E9M22_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C

		.global E9M22_to_int_c
E9M22_to_int_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_to_int_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global int_to_E9M22_c
int_to_E9M22_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   int_to_E9M22_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C





		.global E9M22_add_c
E9M22_add_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_add_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_sub_c
E9M22_sub_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_sub_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_mul_c
E9M22_mul_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_mul_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_div_c
E9M22_div_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_div_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_neg_c
E9M22_neg_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_neg_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_abs_c
E9M22_abs_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_abs_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C





		.global E9M22_are_eq_c
E9M22_are_eq_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_are_eq_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_are_ne_c
E9M22_are_ne_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_are_ne_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_are_unordered_c
E9M22_are_unordered_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_are_unordered_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_is_gt_c
E9M22_is_gt_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_is_gt_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_is_ge_c
E9M22_is_ge_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_is_ge_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_is_lt_c
E9M22_is_lt_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_is_lt_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_is_le_c
E9M22_is_le_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_is_le_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_normalize_and_round_c
E9M22_normalize_and_round_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_normalize_and_round_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global E9M22_round_c
E9M22_round_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   E9M22_round_c_			@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




		.global count_leading_zeros_c
count_leading_zeros_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   count_leading_zeros_c_		@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C


		.global count_trailing_zeros_c
count_trailing_zeros_c:
		push {r1-r3, r12, lr}		@; salvar regs modificables per rutina en C
		bl   count_trailing_zeros_c_	@; cridar rutina en C
		pop  {r1-r3, r12, pc}		@; restaurar regs modificables per rutina en C




@; umul32x32_64():	multiplica 2 operands naturals de 32 bits
@;					i retorna el resultat de 64 bits.
@; declaració en C:
@; 		unsigned long long umul32x32_64(unsigned int a, unsigned int b);

        .global umul32x32_64
umul32x32_64:
        push {r2, lr}
        
        mov r2, r0
        umull r0, r1, r2, r1
        
        pop {r2, pc}


@; umul32x32_2x32(): multiplica 2 operands naturals de 32 bits
@;					 i retorna el resultat de 64 bits.
@; declaració en C:
@; 		void umul32x32_2x32(unsigned int a, unsigned int b,
@;							unsigned int *mulLow, unsigned int *mulHigh );
        .global umul32x32_2x32
umul32x32_2x32:
        push {r4, r5, lr}
        
        umull r4, r5, r0, r1
        str r4, [r2]
        str r5, [r3]
        
        pop {r4, r5, pc}

.end

