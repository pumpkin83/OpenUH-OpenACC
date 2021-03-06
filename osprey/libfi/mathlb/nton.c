/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2.1 of the GNU Lesser General Public License 
  as published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement 
  or the like.  Any license provided herein, whether implied or 
  otherwise, applies only to this software file.  Patent licenses, if
  any, provided herein do not apply to combinations of this program with 
  other software, or any other product whatsoever.  

  You should have received a copy of the GNU Lesser General Public 
  License along with this program; if not, write the Free Software 
  Foundation, Inc., 59 Temple Place - Suite 330, Boston MA 02111-1307, 
  USA.

  Contact information:  Silicon Graphics, Inc., 1600 Amphitheatre Pky,
  Mountain View, CA 94043, or:

  http://www.sgi.com

  For further information regarding this notice, see:

  http://oss.sgi.com/projects/GenInfo/NoticeExplan

*/


#pragma ident "@(#) libfi/mathlb/nton.c	92.3	09/27/99 14:56:21"


#include <fortran.h>
#include <liberrno.h>
#include <math.h>

extern _f_int4 _NTON( _f_int4 x, _f_int4 y );

/*
 * NTON integer*32 raised to an integer*32
 */
_f_int4
_NTON( _f_int4 x,
	   _f_int4 y )
{
	_f_int4 base, result, i;
	if (x == 0) {
		if (y == 0) {
#if	defined(__mips) || defined(_LITTLE_ENDIAN)
			return(1);
#else
			_lerror(_LELVL_ABORT, FEIPOWZR);
#endif
		}
		return(0);
	}
	if (y < 0) {
		result =	0;
		if ((x == 1) || (x == -1)) {
			result =	1;
			if (((y & 1) == 1) && (x == -1))
				result =	-1;
		}
	} else {
		if (y == 0)
			return(1);
		base =	x;
		if (x < 0)
			base =	-x;
		result =	1;
		i =	y;
		while (i != 0) {
			if ((i & 1) == 1)
				result *=	base;
			base *=	base;
			i =	(unsigned) i >> 1;
		};
		if ((x < 0) && ((y & 1) == 1))
			result =	-result;
	}
	return(result);
}

#if	defined(__mips) || defined(_LITTLE_ENDIAN)
_f_int4
__powii( _f_int4 x,
	   _f_int4 y )
{
	return(_NTON(x, y));
}

/* do not turn these on unless needed for compatibility with
 * f77 and pass by address.
 */

#endif	/* end of mips or little endian */
