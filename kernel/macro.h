#pragma once

#define ALIGN(n)         __attribute__((__aligned__(n)))
#define IS_ALIGNED(x, a) (((x) & ((typeof(x))(a)-1)) == 0)

#define ROUND_UP(x, n)     (((x) + (n)-1) & ~((n)-1))
#define ROUND_DOWN(x, n)   ((x) & ~((n)-1))
#define DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))

#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define BIT(x) (1 << (x))
