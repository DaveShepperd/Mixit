#if 0
===========================================================================

H E X U T L . H


	Copyright 1989 Atari Games.  All rights reserved.
	Author: Lyle Rains

	---------------------------------------------------------------------------
	Revision history:

	---------------------------------------------------------------------------
	Known bugs/features/limitations:

	===========================================================================
#endif

#ifndef HEXUTL_H
#define HEXUTL_H

#include "mixit.h"

#define HEXTOBYTE(hex)   ((hex)[1] + ((hex)[0] << 4))

#define XX 0x7F

extern uint8_t chartohex[256];
extern uint8_t hex_of[16];

int			strtobytes(uint8_t *str, int nbytes);
int			strtobytes2(uint8_t *dst, const char *src, int nbytes);
int			strtohex(uint8_t *str, int nchars);
void		hextobytes(uint8_t *hexstr, int nbytes);
LogicalAddr bytestoaddr(uint8_t *bytestr, int nbytes);
LogicalAddr hextoaddr(uint8_t *hexstr, int nnybbles);

#endif /* HEXUTL_H */
