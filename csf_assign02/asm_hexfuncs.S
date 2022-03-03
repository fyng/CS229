/* Assembly language implementation of hexdump functions */

.section .rodata
/* TODO: add read-only data such as strings here */
sHexDigits:	.string "0123456789abcdef"
//sNull:	.string "\0"

.section .text
/* TODO: add functions here
	* asm_hexfuncs.S may call read and write 8
	*
	* Notes: 	xxd 
	* Callee-saved registers: rbx, rbp, rsp, r12-r15
	* Subroutine arguments:  rdi, rsi, rdx, rcx, r8, r9
*/
	.globl hex_read
	.globl hex_write_string
	.globl hex_format_offset
	.globl hex_format_byte_as_hex
	.globl hex_to_printable
	
hex_read:
		pushq %r12 		/* %r12 is n counter. Stack pointer aligned*/
		pushq %r13 		/* %r13 is previous n counter */
		pushq %r14
		movq %rdi, %r14	/* %r14 stores data_buf[] */

		movq %r14, %rsi /* %rsi takes data_buf[] */
		movq $0, %rdi
		movq $16,%rdx	
		call read		/* read (0, data_buff[], 16) */
		movq %rax, %r12  // 13
		cmpq $0, %r12
		je .LhexReadDone 

	.LhexReadLoop:
		cmpq $16, %r12	/* %r12 >= 16 ? */
		jae .LhexReadDone

		movq %r12, %r13 /* %r13 stores %r12 */
		movq $0, %rdi
		leaq (%r14, %r12, 1), %rsi
		movq $16, %rdx 
		subq %r12, %rdx	/* compute 16 - n */
		call read 		/* read(0, databuf + n, 16 - n) */

		addq %rax, %r12 /* add read length to n */
		cmpq %r12, %r13 
		je .LhexReadDone

		jmp .LhexReadLoop

	.LhexReadDone:
		movq %rsi, %rdi	/* %rdi takes data_buf[] */
		movq %r12, %rax /* return the value from r12 to return rax */
		popq %r14
		popq %r13
		popq %r12		/* restore %r12, realign stack pointer */
		ret

hex_write_string:
		subq $8, %rsp
		movq $0, %r10	/* initialize %r10 to 0, for counter */
		//movq $3, %r11	/* set r11 to sNull */
		
	.LhexWriteStringLoop:
		cmpq $16, %r10 	/* r10 >= 16? */
		jae .hexWriteStringLdone
		cmpb $0, (%rdi, %r10, 1)
		je .hexWriteStringLdone

		inc %r10
		jmp .LhexWriteStringLoop

	.hexWriteStringLdone:
		movq %rdi, %rsi
		movq $1, %rdi
		movq %r10, %rdx
		call write
		addq $8, %rsp
		ret

/* 
* %rdi: unsigned offset
* %rsi: char sbuf[]
* %rcx: loop counter
*/
hex_format_offset:
		subq $8, %rsp
		pushq %r12		/* loop counter */
		pushq %r13		/* copy of %rdi (unsigned offset) */
		movq $0, %r12	/* initialise loop counter %r12 */
		movl %edi, %r13d	/* copy unsigned offset to %r13d */

	.LbyteHexLoop:
		movl %r13d, %edi
		cmpq $4, %r12 	/* end loop when %r10 is above equal to 4 */
		jae .LbyteHexDone

		movb %r12b, %cl
		shlb $3, %cl	/* %cl = %cl*8 */
		shll %cl, %edi 	/* %edi << %cl */
		shrl $24, %edi 	/* %edi >> 24 */

		/* call on hex_format_byte_as_hex */
		call hex_format_byte_as_hex
		incq %rsi		
		incq %rsi
		incq %r12		
		jmp .LbyteHexLoop

	.LbyteHexDone:
		movb $0x00, %sil	 	/* add null terminator */

		popq %r13
		popq %r12
		addq $8, %rsp
		ret

/* 
* %rdi: unsigned char byteval
* %rsi: char sbuf[]
* %r10: loop counter
*
*/
hex_format_byte_as_hex:
	subq $8, %rsp
	movq $0, %rax				/* set rax to 0 to clear values */
	movq %rdi, %r8				/* move the byteval to r8 */
	shrb $4, %r8b				/* right shift r8 to get the proper index value */
	andq $0x0f, %rdi			/* based on c code, add the 0 to set rdi */
	movq $sHexDigits, %r9		/* Let r9 take the hexdigits */
	movb (%r9, %r8, 1), %al 	/* sil = r9[rdi] */
	movb %al, 0(%rsi)			/* move the hexadecimal val to rsi or sbuf[0] */
	movb (%r9, %rdi, 1), %al 	/* get the next hexadecimal for the char to print */
	movb %al, 1(%rsi)			/* move the next hex val to rsi or sbuf[1] */
	movb $0x00, 2(%rsi)	 		/* add null terminator */
	
	addq $8, %rsp
	ret

hex_to_printable:
	subq $8, %rsp
	movq $0, %rax

	cmpq $32, %rdi		/* rdi less than 32? */
	jl .LPrintPeriod 	/* jump to .LPrintPeriod */
	cmpq $126, %rdi		/* rdi greater than 126? */
	jg .LPrintPeriod	/* jump to .LPrintPeriod */

	movq %rdi, %rax		/* byteval is good, return val */
	jmp .LPrintDone


	.LPrintPeriod:	
	movb $46, %al 		/* byteval is good, return 46 (period in decimal) */

	.LPrintDone:
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
