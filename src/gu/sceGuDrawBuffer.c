/*
 * PSP Software Development Kit - https://github.com/pspdev
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include "guInternal.h"

void sceGuDrawBuffer(int psm, void *fbp, int frame_width)
{
	gu_draw_buffer.pixel_size = psm;
	gu_draw_buffer.frame_width = frame_width;
	gu_draw_buffer.frame_buffer = fbp;

	if (!gu_draw_buffer.depth_buffer && gu_draw_buffer.height)
		gu_draw_buffer.depth_buffer = (void *)(((unsigned int)fbp) + (unsigned int)((gu_draw_buffer.height * frame_width) << 2));

	if (!gu_draw_buffer.depth_width)
		gu_draw_buffer.depth_width = frame_width;

	sendCommandi(FRAMEBUF_PIX_FORMAT, psm);
	sendCommandi(FRAME_BUF_PTR, (unsigned int)gu_draw_buffer.frame_buffer);
	sendCommandi(FRAME_BUF_WIDTH, ((((unsigned int)gu_draw_buffer.frame_buffer) & 0xff000000) >> 8) | gu_draw_buffer.frame_width);
	sendCommandi(Z_BUF_PTR, (unsigned int)gu_draw_buffer.depth_buffer);
	sendCommandi(Z_BUF_WIDTH, ((((unsigned int)gu_draw_buffer.depth_buffer) & 0xff000000) >> 8) | gu_draw_buffer.depth_width);
}
