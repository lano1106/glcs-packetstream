/*
 * optimization.h
 *
 * Olivier Langlois - Sept 27, 2013
 */

#ifndef __OPTIMIZATION_H__
#define __OPTIMIZATION_H__

#ifndef likely
#define likely(x)	__builtin_expect (!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)	__builtin_expect (!!(x), 0)
#endif

#endif

