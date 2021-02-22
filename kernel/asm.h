#pragma once

#define BEGIN_FUNC(_name)        \
	.global _name;           \
	.type _name, % function; \
	_name:

#define END_FUNC(_name) .size _name, .- _name

#define EXPORT(symbol) \
	.globl symbol; \
	symbol:

#define BEGIN_FUNC_STATIC(_name) \
	.type _name, % function; \
	_name:

#define END_FUNC_STATIC(_name) .size _name, .- _name

#define LOCAL_DATA(x) \
	.type x, 1;   \
	x:
#define DATA(x)    \
	.global x; \
	.hidden x; \
	LOCAL_DATA(x)
#define END_DATA(x) .size x, .- x

/*
 * LEAF - declare leaf routine
 */
#define LEAF(symbol)             \
	.globl symbol;           \
	.align 2;                \
	.type symbol, @function; \
	.ent symbol, 0;          \
	symbol:                  \
	.frame sp, 0, ra

/*
 * NESTED - declare nested routine entry point
 */
#define NESTED(symbol, framesize, rpc) \
	.globl symbol;                 \
	.align 2;                      \
	.type symbol, @function;       \
	.ent symbol, 0;                \
	symbol:                        \
	.frame sp, framesize, rpc

/*
 * END - mark end of function
 */
#define END(function)  \
	.end function; \
	.size function, .- function

#define EXPORT(symbol) \
	.globl symbol; \
	symbol:

#define FEXPORT(symbol)          \
	.globl symbol;           \
	.type symbol, @function; \
	symbol:
