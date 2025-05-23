/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include "guInternal.h"

void sceGuClutMode(unsigned int cpsm, unsigned int shift, unsigned int mask, unsigned int csa)
{
	unsigned int argument = (cpsm) | (shift << 2) | (mask << 8) | (csa << 16);
	sendCommandi(CLUT_FORMAT, argument);
}
