#include "timer.h"
#include "types.h"

#define TICK_RATE_HZ 2

u64 cur_cnt;
u64 cur_freq;
u32 cnt_tval;
u32 cnt_ctl;

static void plat_timer_init()
{
	/* Get the frequency and init count */
	asm volatile("mrs %0, cntvct_el0" : "=r"(cur_cnt));
	asm volatile("mrs %0, cntfrq_el0" : "=r"(cur_freq));

	/* Calculate the tv */
	cnt_tval = (cur_freq / TICK_RATE_HZ);

	/* set the timervalue here */
	asm volatile("msr cntv_tval_el0, %0" ::"r"(cnt_tval));

	/* Set the control register */
	cnt_ctl = 0x1;
	asm volatile("msr cntv_ctl_el0, %0" ::"r"(cnt_ctl));
}

static void plat_handle_timer_irq()
{
	asm volatile("msr cntv_tval_el0, %0" ::"r"(cnt_tval));
	asm volatile("msr cntv_ctl_el0, %0" ::"r"(cnt_ctl));
}

// static void plat_disable_timer()
// {
// 	u64 timer_ctl = 0x0;
// 	asm volatile("msr cntv_ctl_el0, %0" ::"r"(timer_ctl));
// }

// static void plat_enable_timer()
// {
// 	u64 timer_ctl = 0x1;
// 	asm volatile("msr cntv_tval_el0, %0" ::"r"(cnt_tval));
// 	asm volatile("msr cntv_ctl_el0, %0" ::"r"(timer_ctl));
// }

void timer_init()
{
	plat_timer_init();
}

void handle_timer_irq()
{
	plat_handle_timer_irq();
}
