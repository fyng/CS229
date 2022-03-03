/* Hexdump main function, assembly language version */
	.section .rodata
sColonSpace:	.string ": "
sSingleSpace:	.string " "
sTripleSpace:	.string "   "
sNewLine:		.string "\n"

	.data
data_buf:	.space 16
sbuf:	.space 9
	
/*
	* Callee-saved registers: rbx, rbp, rsp, r12-r15
	* Subroutine arguments:  rdi, rsi, rdx, rcx, r8, r9
*/

	.section .text
	
	.globl main
main:
	subq $8, %rsp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	pushq %rbx
	pushq %rbp

	movl $0, %r15d	/* r15 hold the offset */
	movq $data_buf, %r14	/* r14 hold data_buf */
	movq $sbuf, %r13	/* r13 hold sbuf */
	movq $0, %rbx	/* Let rbx be temporary constant */
	movq $0, %rbp	/* Let rbp represent for loop counters */
	movq $0, %r9 	/* Let r9 help dereference variables */
	movq $0, %rsi	/* Clear rsi */

	movq %r14, %rdi	/* move data_buf into rdi */
	call hex_read	/* Call hex_read and let it fill data_buf */
	movq %rdi, %r14	/* move data_buf back to %r14 */
	movq %rax, %r12	/* r12 store count (# characters read) */

.LProgramLoop:
	cmpq $0, %r12	/* See if count is 0 */
	je .LProgramEnd	/* If so, end program */

	movl %r15d, %edi	/* Move offset to %rdi */
	movq %r13, %rsi	/* Move sbuf to %rsi */
	call hex_format_offset
	addl $16, %r15d	/* Add 16 to offset */

	movq %r13, %rdi	/* Move sbuf to %rdi */
	call hex_write_string
	movq $sColonSpace, %rdi	/* Move ": " to %rdi*/
	call hex_write_string

.LHexStringStart:
	cmpq %rbp, %r12	/* counter <= count ? */
	jbe .LHexStringEnd	/* If so, end current for loop */
	movb (%r14, %rbp, 1), %dil /* Move data_buf[i] to argument position 1*/
	movq %r13, %rsi	/* Move sbuf to argument position 2 */
	call hex_format_byte_as_hex
	movq %rsi, %r13	/* Update changes to sbuf */
	movq %r13, %rdi	/* Move sbuf to argument position 1 */
	call hex_write_string
	movq $sSingleSpace, %rdi	/* Move " " to %rdi */
	call hex_write_string
	movb (%r14, %rbp, 1), %dil
	call hex_to_printable
	movb %al, (%r14, %rbp, 1)
	inc %rbp	/* Increment loop counter rbp */
	jmp .LHexStringStart
	
.LHexStringEnd:
	movq $0, %rbp	/* Reset loop counter to 0 */

.LHexStringSpaceStart:
	movq $16, %rbx
	subq %r12, %rbx
	cmpq %rbp, %rbx /* counter <= (16-count) ?*/
	jbe .LHexStringSpaceEnd	/* If so, end current for loop */
	movq $sTripleSpace, %rdi	/* Move "   " to %rdi */
	call hex_write_string
	inc %rbp	/* Increment loop counter rbp */
	jmp .LHexStringSpaceStart
	
.LHexStringSpaceEnd:
	movq $0, %rbp	/* Reset loop counter to 0 */
	movq $sSingleSpace, %rdi	/* Move " " to %rdi */
	call hex_write_string
	movq %r14, %rdi	/* Move data_buf to %rdi */
	call hex_write_string

.LResetBufferStart:
	cmpq $16, %rbp	/* counter <= 16 ? */
	jae .LResetBufferEnd	/* If so, end current for loop */
	movb $0x00, (%r14, %rbp, 1)
	inc %rbp	/* Increment loop counter rbp */
	jmp .LResetBufferStart
	
.LResetBufferEnd:
	movq $0, %rbp	/* Reset loop counters */
	movq $sNewLine, %rdi	/* Move constant character to rbx */
	call hex_write_string

	cmpq $16, %r12 /* counter >= 16 ? */
	jae .LContinue	/* If so, continue reading */
	movq $0, %r12	/* If not, set count to 0 */
	jmp .LProgramLoop /* Go back to top of loop */
	
.LContinue:
	movq %r14, %rdi	/* Move data_buf to argument position 1 */
	call hex_read
	//movq %rdi, %r14	/* Update data_buf for any changes */
	movq %rax, %r12 /* Reupdate counter based on hex_read output */
	jmp .LProgramLoop

.LProgramEnd:	
	movq $0, %rax	/* main returns 0 */
	popq %rbp
	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
