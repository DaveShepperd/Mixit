#ifndef MIXIT_H
#define MIXIT_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#if defined(WIN32)
#include <io.h>
#endif
#if LINUX
#include <unistd.h>
#endif

#include "formats.h"

typedef uint32_t		LogicalAddr;

extern int noisy;
extern int debug;
extern int noDate;

#define DOING_UNDEFINED	(0)
#define DOING_IN_CMD	(1)
#define DOING_OUT_CMD	(2)
#define DOING_OUTPUT	(4)
extern int whatWeAreDoing;

extern FILE *errFile;

#define in(l,m,h)   			( ((l) <= (m))  &&  ((m) <= (h)))
#define byte_of(x) 				( (x) & 0xFF )
#define PUT_BUF( ptr, byte )    ( *ptr++ = (uint8_t)byte )

#include "port.h"
#include "image.h"
#include "gpf.h"
#include "formats.h"
#include "hexutl.h"
#include "prototyp.h"

extern BUF  inspec;                 /* accepted file specs for MIXIT routines*/
extern BUF  outspec;                /* accepted file specs for MIXIT routines*/
extern GPF  outgpf;                 /* environment for getfile() / putfile() */
extern BUF  filespec;               /* filespec we will collect EXACTLY one of*/

extern char *error2str( int num );
#endif
