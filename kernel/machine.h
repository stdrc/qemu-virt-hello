#pragma once

#define COUNTER_FREQ_IN_HZ 0x3b9aca0

#define GIC_BASE (0x8000000)

#define GICD_BASE (GIC_BASE + 0x00000)
#define GICC_BASE (GIC_BASE + 0x10000)

/* GICD Registers */
#define GICD_CTLR         (GICD_BASE + 0x000)
#define GICD_TYPER        (GICD_BASE + 0x004)
#define GICD_IIDR         (GICD_BASE + 0x008)
#define GICD_IGROUPR      (GICD_BASE + 0x080)
#define GICD_ISENABLER    (GICD_BASE + 0x100)
#define GICD_ICENABLER    (GICD_BASE + 0x180)
#define GICD_ISPENDR      (GICD_BASE + 0x200)
#define GICD_ICPENDR      (GICD_BASE + 0x280)
#define GICD_ISACTIVER    (GICD_BASE + 0x300)
#define GICD_ICACTIVER    (GICD_BASE + 0x380)
#define GICD_IPRIORITYR   (GICD_BASE + 0x400)
#define GICD_ITARGETSR    (GICD_BASE + 0x800)
#define GICD_ICFGR        (GICD_BASE + 0xC00)
#define GICD_PPISR        (GICD_BASE + 0xD00)
#define GICD_SGIR         (GICD_BASE + 0xF00)
#define GICD_SGIR_CLRPEND (GICD_BASE + 0xF10)
#define GICD_SGIR_SETPEND (GICD_BASE + 0xF20)

/* GICC Registers */
#define GICC_CTLR (GICC_BASE + 0x0000)
#define GICC_PMR  (GICC_BASE + 0x0004)
#define GICC_BPR  (GICC_BASE + 0x0008)
#define GICC_IAR  (GICC_BASE + 0x000C)
#define GICC_EOIR (GICC_BASE + 0x0010)
#define GICC_APR  (GICC_BASE + 0x00D0)
#define GICC_IIDR (GICC_BASE + 0x00FC)
#define GICC_DIR  (GICC_BASE + 0x1000)

#define GICV_CTLR (GICV_BASE + 0x0)

#define GICD_CTL_ENABLE         0x1
#define GICD_CTL_DISABLE        0x0
#define GICD_INT_ACTLOW_LVLTRIG 0x0
#define GICD_INT_EN_CLR_X32     0xffffffff
#define GICD_INT_EN_SET_SGI     0x0000ffff
#define GICD_INT_EN_CLR_PPI     0xffff0000
#define GICD_INT_DEF_PRI        0xa0
#define GICD_INT_DEF_PRI_X4                                    \
	((GICD_INT_DEF_PRI << 24) | (GICD_INT_DEF_PRI << 16) | \
	 (GICD_INT_DEF_PRI << 8) | GICD_INT_DEF_PRI)

/* Register bits */
#define GICD_TYPE_LINES      0x01F
#define GICD_TYPE_CPUS_SHIFT 5
#define GICD_TYPE_CPUS       0x0E0
#define GICD_TYPE_SEC        0x400

#define GICC_ENABLE            0x1
#define GICC_INT_PRI_THRESHOLD 0xf0

#define GICC_CTRL_EOImodeNS_SHIFT 9
#define GICC_CTRL_EOImodeNS       (1 << GICC_CTRL_EOImodeNS_SHIFT)

#define GICC_IAR_INT_ID_MASK 0x3ff
#define GICC_INT_SPURIOUS    1023
#define GICC_DIS_BYPASS_MASK 0x1e0

#define GIC_PRI_IRQ 0xA0
#define GIC_PRI_IPI 0x90

/* GICD_SGIR defination */
#define GICD_SGIR_SGIINTID_SHIFT   0
#define GICD_SGIR_CPULIST_SHIFT    16
#define GICD_SGIR_LISTFILTER_SHIFT 24
#define GICD_SGIR_VAL(listfilter, cpulist, sgi)         \
	(((listfilter) << GICD_SGIR_LISTFILTER_SHIFT) | \
	 ((cpulist) << GICD_SGIR_CPULIST_SHIFT) |       \
	 ((sgi) << GICD_SGIR_SGIINTID_SHIFT))

#define GIC_INTID_EL1_PHYS_TIMER 30
#define GIC_INTID_EL3_PHYS_TIMER 29
#define GIC_INTID_VIRT_TIMER     27
#define GIC_INTID_EL2_PHYS_TIMER 26
