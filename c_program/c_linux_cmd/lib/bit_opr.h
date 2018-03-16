#ifndef _BIT_OPE_H_
#define _BIT_OPE_H_

#define BIT_SET(integer, offset)        ((integer) |= 1 << (offset))
#define BIT_CLR(integer, offset)        ((integer) &= ~(1 << (offset)))
#define BIT_ISSET(integer, offset)      ((integer) & (1 << (offset)))

#endif
