#include "asm.h"
#include "irq_entry.h"

/*
 * DAIF 4 bits
 * D: debug exceptions
 * A: asynchronous aborts SError
 * I: IRQs
 * F: FIQs
 */
BEGIN_FUNC(enable_irq)
	msr	daifclr, #2
	ret
END_FUNC(enable_irq)

BEGIN_FUNC(disable_irq)
	msr	daifset, #2
	ret
END_FUNC(disable_irq)
