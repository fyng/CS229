/* Hexdump main function, assembly language version */
	.section .rodata
sColonSpace:	.string ": "
sSingleSpace:	.string " "
sTripleSpace:	.string "   "
sNullSpace:	.string "\0"
sNewLine:	.string "\n"

	.section .text
	
	.globl main
main:
	movq $0, %r15	/* Let r15 hold the offset */
	movq $0, %r14	/* Let r14 hold data_buf */
	movq $0, %r13	/* Let r13 hold sbuf */
	movq $0, %rbx	/* Let rbx be any print constants */
	movq $0, %rbp	/* Let rbp represent for loop counters */
	movq $0, %r9 	/* Let r9 help dereference variables */
	call hex_read	/* Call hex_read and let it fill data_buf */
	movq %rdi, %r14	/* Move first argument back to %r14 */
	movq %rax, %r12	/* Place return value to r12 */

.LProgramLoop:
	cmpq $0, %r12	/* See if count is 0 */
	je .LProgramEnd	/* If so, end program */
	movq %r15, %rdi	/* Move offset to argument position 1 */
	movq %r13, %rsi	/* Move sbuf to argument position 2 */
	call hex_format_offset
	movq %rdi, %r15	/* Updata changes to offset */
	movq %rsi, %r13	/* Update changes to sbuf */
	movq %r13, %rdi	/* Move sbuf to argument position 1 */
	call hex_write_string
	movq $sColonSpace, %rbx	/* Place constant character to rbx */
	movq %rbx, %rdi	/* Move constant rbx to argument position 1 */
	addq $16, %r15	/* Add 16 to offset */

.LHexStringStart:
	cmpq %rbp, %r12	/* See if counter is less than or equal to count */
	jle .LHexStringEnd	/* If so, end current for loop */
	movb (%r14, %rbp, 1), %dil /* Move data_buf[i] to argument position 1*/
	movq %r13, %rsi	/* Move sbuf to argument position 2 */
	call hex_format_byte_as_hex
	movq %rsi, %r13	/* Update changes to sbuf */
	movq %r13, %rdi	/* Move sbuf to argument position 1 */
	call hex_write_string
	movq $sSingleSpace, %rbx	/* Place constant character to rbx */
	movq %rbx, %rdi	/* Move constant rbx to argument position 1 */
	call hex_write_string
	inc %rbp	/* Increment loop counter rbp */
	jmp .LHexStringStart
	
.LHexStringEnd:
	movq $0, %rbp	/* Reset loop counter to 0 */

.LHexStringSpaceStart:
	pushq %r15	/* Use r15 temporarily to determine for loop end */
	movq $16, %r15
	subq %r12, %r15
	cmpq %rbp, %r15 /* See if counter is less than or equal to count */
	jle .LHexStringSpaceEnd	/* If so, end current for loop */
	movq $sTripleSpace, %rbx	/* Move constant character to rbx */
	movq %rbx, %rdi	/* Move constant rbx to argument position 1 */
	call hex_write_string
	inc %rbp	/* Increment loop counter rbp */
	jmp .LHexStringSpaceStart
	
.LHexStringSpaceEnd:
	movq $0, %rbp	/* Reset loop counter to 0 */
	popq %r15	/* Restore r15 to previous values */

	movq $sSingleSpace, %rbx	/* Move constant character to rbx */
	movq %rbx, %rdi	/* Move constant rbx to argument position 1 */
	call hex_write_string
	movq %r14, %rdi	/* Move data_buf to argument position 1 */
	call hex_write_string

.LResetBufferStart:
	pushq %r15	/* Push r15 to use it to hold 16 */
	movq $16, %r15	
	cmpq %rbp, %r15	/* See if counter is less than or equal to 16 */
	
	jle .LResetBufferEnd	/* If so, end current for loop */

	// Following attempt to move a value to 
	pushq %r15
	movq $sNullSpace, %r15
	movb %r15b, (%r14, %rbp, 1)
	popq %r15

	inc %rbp	/* Increment loop counter rbp */
	jmp .LResetBufferStart
	
.LResetBufferEnd:
	movq $0, %rbp	/* Reset loop counters */
	popq %r15	/* Reset r15 back to what it should be */

	movq $sNewLine, %rbx	/* Move constant character to rbx */
	movq %rbx, %rdi	/* Move constant rbx to argument position 1 */
	call hex_write_string

	cmpq $16, %r12 /* See if count is greater than or equal to 16 */
	jge .LContinue	/* If so, continue reading */
	movq $0, %r12	/* If not, set count to 0 */
	jmp .LProgramLoop /* Go back to top of loop */
	
.LContinue:
	movq %r14, %rdi	/* Move data_buf to argument position 1 */
	call hex_read
	movq %rdi, %r14	/* Update data_buf for any changes */
	movq %rax, %r12 /* Reupdate counter based on hex_read output */
	jmp .LProgramLoop

.LProgramEnd:	
	ret

/* vim:ft=gas:
 */
