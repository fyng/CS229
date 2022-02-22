/* Assembly language implementation of hexdump functions */

	.section .rodata
	/* TODO: add read-only data such as strings here */
	sHexDigits: .string "0123456789abcdef"


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
		subq $8, %rsp
		/* TODO: implement function */
		addq $8, %rsp
		ret

	hex_write_string:
		subq $8, %rsp
		/* TODO: implement function */
		addq $8, %rsp
		ret

	/* 
	* %rdi: unsigned offset
	* %rsi: char sbuf[]
	* %rcx: loop counter
	*/
	hex_format_offset:
		subq $8, %rsp
		movq $0, %rcx

	.LbyteHexLoop:
		movq %rdi, %r8 /* %r8 stores unsigned offset */
		cmpq $4, %rcx
		jae .LbyteHexDone
		/*movq %ecx, %r11 /* copy counter value to %r11 */
		shlq $3, %rcx
		shrq %cl, %r8
		shlq $24, %r8
		shlq $3, %rcx

		movb %r8b, (%rsi, %rcx, 2)	/* copy 1 byte value from %r8 to sbuf */ 
		inc %rcx

	.LbyteHexDone:
		movb $0, (%rsi, %rcx, 2) /* add null terminator */
		movq %rsi, %rax /* copy to function return */
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
	//movb %al, %ah/* copy byte in %al to %ah */
	movq $0, %rax
	movq %rdi, %r8
	shrb $4, %r8b 				/* ah takes higher tibble */
	andq $0x0f, %rdi			/* rsi takes lower tibble */
	
	movq $sHexDigits, %r9
	movb (%r9, %r8, 1), %al 	/* sil = r9[rdi] */
	//pushb %al
	movb %al, 0(%rsi)
	//pop %sil
	//add $1, (%sil)
	
	//shlq $4, %rax
	//orq %rdi, %rax
	movb (%r9, %rdi, 1), %al 	/* sil = r9[rdi] */
	//push %al
	//pop %sil
	movb %al, 1(%rsi)
	//add $1, (%sil)

	//incb %al
	//movb (%r9, %rdi, 1), %al	
	//incb %al
	movb $0x00, 2(%rsi)	 			/* add null terminator */
	
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
