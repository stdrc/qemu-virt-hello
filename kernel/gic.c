#include "gic.h"
#include "types.h"
#include "machine.h"
#include "tools.h"
#include "printf.h"
#include "timer.h"

static u32 gicv2_get_cpumask()
{
	u32 mask, i;
	for (i = mask = 0; i < 32; i += 4) {
		mask = get32(GICD_ITARGETSR + i);
		mask |= mask >> 16;
		mask |= mask >> 8;
		if (mask)
			break;
	}
	return mask;
}

static void gicv2_dist_init()
{
	u32 cpumask;
	u32 type;
	u32 nr_lines;
	int i;

	/* Disable the distributor */
	put32(GICD_CTLR, GICD_CTL_DISABLE);

	printf("GICD_IGROUPR: %x\n", get32(GICD_IGROUPR));

	type = get32(GICD_TYPER);
	nr_lines = get32(GICD_TYPER) & GICD_TYPE_LINES;
	nr_lines = (nr_lines + 1) * 32;

	printf("GICv2: %d irq(s), %d cpu(s)\n", nr_lines,
	       1 + ((type & GICD_TYPE_CPUS) >> 5));

	/* Set all global interrupts to this CPU only */
	cpumask = gicv2_get_cpumask();
	cpumask |= cpumask << 8;
	cpumask |= cpumask << 16;
	printf("cpumask: %u\n", cpumask);
	printf("Set GICD_ITARGETSR\n");
	for (i = 32; i < nr_lines; i += 4) {
		put32(GICD_ITARGETSR + i * 4 / 4, cpumask);
		printf("[0x%lx] = 0x%x\n", GICD_ITARGETSR + i * 4 / 4,
		       get32(GICD_ITARGETSR + i * 4 / 4));
	}

	/* Set all global interrupts to be level triggered, active low */
	printf("Set GICD_ICFGR\n");
	for (i = 32; i < nr_lines; i += 16) {
		put32(GICD_ICFGR + i / 4, GICD_INT_ACTLOW_LVLTRIG);
		printf("[0x%lx] = 0x%x\n", GICD_ICFGR + i / 4,
		       get32(GICD_ICFGR + i / 4));
	}

	/* Set priority on all global interrupts */
	printf("Set GICD_IPRIORITYR\n");
	for (i = 32; i < nr_lines; i += 4) {
		put32(GICD_IPRIORITYR + i, GICD_INT_DEF_PRI_X4);
		printf("[0x%lx] = 0x%x\n", GICD_IPRIORITYR + i,
		       get32(GICD_IPRIORITYR + i));
	}

	/*
	 * Deactivate and disable all SPIs. Leave the PPI and SGIs
	 * alone as they are in the redistributor registers on GICv3.
	 */
	printf("Deactivate and disable all SPIs\n");
	for (i = 32; i < nr_lines; i += 32) {
		put32(GICD_ICACTIVER + i / 8, GICD_INT_EN_CLR_X32);
		put32(GICD_ICENABLER + i / 8, GICD_INT_EN_CLR_X32);
	}

	/* Turn on the distributor */
	put32(GICD_CTLR, GICD_CTL_ENABLE);
	printf("GICD enabled, GICD_CTLR: 0x%x\n", get32(GICD_CTLR));
}

static void gicv2_cpu_init()
{
	int i;
	u32 bypass;

	/*
	 * Deal with the banked PPI and SGI interrupts - disable all
	 * private interrupts. Make sure everything is deactivated.
	 */
	printf("Deactivate and disable all PPIs\n");
	for (i = 0; i < 32; i += 32) {
		put32(GICD_ICACTIVER + i / 8, GICD_INT_EN_CLR_X32);
		put32(GICD_ICENABLER + i / 8, GICD_INT_EN_CLR_X32);
	}

	/* Set priority on PPI and SGI interrupts */
	printf("Set GICD_IPRIORITYR\n");
	for (i = 0; i < 32; i += 4) {
		put32(GICD_IPRIORITYR + i * 4 / 4, GICD_INT_DEF_PRI_X4);
		printf("[0x%lx] = 0x%x\n", GICD_IPRIORITYR + i * 4 / 4,
		       get32(GICD_IPRIORITYR + i * 4 / 4));
	}

	/* Ensure all SGI interrupts are now enabled */
	printf("Enable all SGI\n");
	put32(GICD_ISENABLER, GICD_INT_EN_SET_SGI);
	printf("GICD_ISENABLER: 0x%x\n", get32(GICD_ISENABLER));

	/* Don't mask by priority */
	put32(GICC_PMR, GICC_INT_PRI_THRESHOLD);
	printf("GICC_PMR: 0x%x\n", get32(GICC_PMR));
	/* Finest granularity of priority */
	put32(GICC_BPR, 0);
	printf("GICC_BPR: 0x%x\n", get32(GICC_BPR));
	printf("Set GICC_APR\n");
	for (i = 0; i < 4; i++) {
		put32(GICC_APR + i * 4, 0);
		printf("[0x%lx] = 0x%x\n", GICC_APR + i * 4,
		       get32(GICC_APR + i * 4));
	}

	/* Turn on delivery */
	bypass = get32(GICC_CTLR);
	bypass &= GICC_DIS_BYPASS_MASK;
	put32(GICC_CTLR, bypass | GICC_CTRL_EOImodeNS | GICC_ENABLE);
	printf("GICC enabled, GICC_CTLR: 0x%x\n", get32(GICC_CTLR));
}

void gicv2_init()
{
	u32 cpuid = 0; // only core 0 is used

	if (cpuid == 0)
		gicv2_dist_init();

	/* init the cpu interface (GICC) */
	gicv2_cpu_init();

	/* enable the timer's irq */
	put32(GICD_ISENABLER, GICD_INT_EN_CLR_PPI);
	printf("GICD_ISENABLER: %x\n", get32(GICD_ISENABLER));
}

void gicv2_handle_irq()
{
	unsigned int irqnr = 0;
	unsigned int irqstat = 0;

	irqstat = get32(GICC_IAR);
	irqnr = irqstat & 0x3ff;

	printf("irq %d\n", irqnr);
	switch (irqnr) {
	case GIC_INTID_EL1_PHYS_TIMER:
	case GIC_INTID_VIRT_TIMER:
		handle_timer_irq();
		break;
	default:
		break;
	}
	put32(GICC_EOIR, irqstat);
	put32(GICC_DIR, irqstat);
}
