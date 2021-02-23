#include "asm.h"
#include "machine.h"

.section ".text.boot"

BEGIN_FUNC(_start)
	mrs	x1, mpidr_el1
	and	x1, x1, #3
	cmp	x1, #0
	bne	hang

primary:
	bl	arm64_elX_to_el1

	// set exception vector
	adr	x0, exception_vector
	msr	vbar_el1, x0

	// setup stack
	ldr	x0, =stack
	mov	sp, x0

	// jump to C code
	b	main
	// never return

hang:
	wfi
	b	hang
END_FUNC(_start)
