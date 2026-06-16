#if 0
/*===========================================================================
	D L D . C

	Declarations and definitions:

	Rockwell 6502 format records.

	The data is recorded as a series of ASCII records. The format of the
	records is as follows:

	   ;ccaaaadddd...chks

	where ";" is the record sentinal, "cc" is the count of the number of binary
	bytes of data in the record, "aaaa" is the 16 bit load address of byte 0
	in the record, "dd" are bytes of data and "chks" is the 16-bit checksum
	of the count, address, and data converted to bytes.

	All binary bytes in the record are checksumed with 8-bit precision
	including the count and each byte of the 2 byte load address, and the type
	field. All values are expressed in hex and there is 1 ASCII character for
	each 4 bit nibble.

	Whitespace is ignored.

	Copyright 1989 Atari Games.  All rights reserved.
	Author: Lyle Rains

---------------------------------------------------------------------------
Revision history:

---------------------------------------------------------------------------
Known bugs/features/limitations:

===========================================================================*/
#endif

/* #define SHOSTUFF */

#include "mixit.h"

/*==========================================================================*/
int GetRec_dld(InRecord *rec)
{
	int		cnt, datacnt, c;
	uint16_t chk, cksum;
	char	*lookahead, *tmpBufend, *tmpBuf = rec->tmpBuf;
	uint8_t	*inbuf = rec->recBuf;
	uint8_t	*bufend,*data;
	
	if ( fgets(tmpBuf, rec->recBufLen, rec->recFile) == NULL )
	{
		rec->recLen = 0;
		if ( !feof(rec->recFile) )
		{
#if LINUX
			moan("dld.c: error reading record: %s", strerror(errno));
#else
			moan("dld.c: error %d reading record", errno);
#endif
			return (rec->recType = REC_ERR);
		}
		return (rec->recType = REC_EOF);
	}

	/*	Convert to 'pure' hexidecimal record:  purge everything until a ';',
	 *	squeeze out junk, and quit when you find end of record.
	 */

	lookahead = tmpBufend = tmpBuf;
	c = 0;
	while ( *lookahead != 0 )
	{
		if ( c )
		{
			if ( isxdigit(*tmpBufend = *lookahead) )
				++tmpBufend;
		}
		else if ( *lookahead == ';' )
			c = 1;
		if ( *lookahead == '\n' )
		{
			*tmpBufend = 0;
			break;
		}
		++lookahead;
	}
	if ( (cnt = tmpBufend - tmpBuf) == 0 )
	{
		rec->recLen = 0;
		return (rec->recType = REC_UNKNOWN)/* empty line */;
	}
	cnt /= 2;

	/* Convert to byte string. */

	SHOW( *bufend = 0;
		 fputs(tmpBuf, errFile);
		 putc('\n', errFile);
		)
	/* Convert 'cnt' bytes of inbuf from ASCII hex to binary */
	if ( strtobytes2(inbuf, tmpBuf, cnt) )
	{
		SHOW( fprintf(errFile,"dld.c: Record syntax error-> ;%s\n", tmpBuf) )
		rec->recLen = 0;
		return (rec->recType = REC_UNKNOWN);
	}
	bufend = inbuf + cnt;
	SHOW(	for ( data = inbuf; data < bufend; ++data )
			 fprintf(errFile, "%02X", *data);
		 putc('\n', errFile);
		 )
	/* Get count, addr, and record type. */

	datacnt = inbuf[0];
	rec->recSAddr = bytestoaddr(&inbuf[1], 2);
	SHOW( fprintf(errFile, "addr = %04X\n", rec->recSAddr);
		 )

	/* Verify record length and checksum. */

	if ( datacnt > cnt - 5 /* 1-byte count, 2-byte addr, 2-bytes chksum */ )
	{
		moan("dld.c: Record length error (sez %d, is %d)-> ;%s", datacnt, cnt - 5, tmpBuf);
		rec->recLen = 0;
		return (rec->recType = REC_ERR);
	}
	chk = 0;
	for ( data = inbuf; data < bufend - 2; ++data )
		chk += *data;
	SHOW( fprintf(errFile, "chk = 0x%04X\n", chk);
		 )
	cksum = bytestoaddr(bufend - 2, 2);
	if ( chk != cksum )
	{
		if ( datacnt )
		{
			moan("dld(): checksum error. Record has 0x%04X, computed 0x%04X-> ;%s", cksum, chk, tmpBuf);
			rec->recLen = 0;
			return (rec->recType = REC_ERR);
		}
	}
	rec->recLen = datacnt;
	rec->recData = inbuf + 3 /* skip header */;
	if ( datacnt )
		rec->recEAddr = rec->recSAddr + datacnt - 1;
	else
		rec->recEAddr = rec->recSAddr;
	return (rec->recType = datacnt ? REC_DATA : REC_XFER);
} /* end GetRec_dld */

/*==========================================================================*
 * Outputs a single record in DLD format.
 *==========================================================================*/
int PutRec_dld(FILE *file, uint8_t *data, int recsize, uint32_t recstart)
{
	int		j;
	char	*cp;
	char	outbuf[512];
	uint32_t	cksum;
#if 0
	int		recbytes;

	recbytes = recsize + 3;
#endif
	cp  = outbuf;
	sprintf(cp, ";%02X%04X", recsize, recstart & 0xFFFF);
	cp += strlen(cp);

	/* Append data to the record */
	cksum = recsize + ((recstart >> 8) & 0xFF) + (recstart & 0xFF);
	for ( j = 0; j < recsize; ++j )
	{
		cksum += *data;
		cp     = to_hex(*data++, cp);
	}       /* end each record */
	/* Append the checksum */
	sprintf(cp, "%04X\n", cksum & 0xFFFF);
	fputs(outbuf, file);    /* Output the record to the file */

	return 1;

} /* end PutRec_dld */

/*==========================================================================*
 * Outputs all footer information required by a DLD format file.
 *==========================================================================*/
int PutFoot_dld(FILE *file)
{
	fputs(";0000000000\n", file);
	return 1;
} /* end PutFoot_dld */


