#include "irq_entry.h"
#include "types.h"
#include "irq.h"
#include "printf.h"
#include "esr.h"
#include "machine.h"
#include "tools.h"
#include "timer.h"
#include "gic.h"

static void handle_irq();

static const char *entry_type_strings[] = {
	[SYNC_EL1t] = "SYNC_EL1t", [IRQ_EL1t] = "IRQ_EL1t",
	[FIQ_EL1t] = "FIQ_EL1t",   [ERROR_EL1t] = "ERROR_EL1t",
	[SYNC_EL1h] = "SYNC_EL1h", [IRQ_EL1h] = "IRQ_EL1h",
	[FIQ_EL1h] = "FIQ_EL1h",   [ERROR_EL1h] = "ERROR_EL1h",
};

void handle_entry_c(int type, u64 esr, u64 address)
{
	u32 esr_ec = GET_ESR_EL1_EC(esr);
	printf("Interrupt type: %s, ESR: 0x%lx, Fault address: 0x%lx, EC 0x%x\n",
	       entry_type_strings[type], esr, address, esr_ec);

	switch (type) {
	case IRQ_EL1t:
	case IRQ_EL1h:
		handle_irq();
		break;
	default:
		break;
	}
}

static void handle_irq()
{
	printf("handle_irq\n");
	print_daif(); // PSTATE.I should be 1, DAIF = 0x3c0
	gicv2_handle_irq();
	enable_irq();
	printf("IRQ re-enabled\n");
	print_daif(); // PSTATE.I should be 0, DAIF = 0x340
}
