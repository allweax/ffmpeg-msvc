/*
 * RV10 encoder
 * Copyright (c) 2000,2001 Fabrice Bellard
 * Copyright (c) 2002-2004 Michael Niedermayer
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * RV10 encoder
 */

#include "mpegvideo.h"
#include "put_bits.h"

void rv10_encode_picture_header(MpegEncContext *s, int picture_number)
{
    int full_frame= 0;

    align_put_bits(&s->pb);

    put_bits(&s->pb, 1, 1);     /* marker */

    put_bits(&s->pb, 1, (s->pict_type == FF_P_TYPE));

    put_bits(&s->pb, 1, 0);     /* not PB frame */

    put_bits(&s->pb, 5, s->qscale);

    if (s->pict_type == FF_I_TYPE) {
        /* specific MPEG like DC coding not used */
    }
    /* if multiple packets per frame are sent, the position at which
       to display the macroblocks is coded here */
    if(!full_frame){
        put_bits(&s->pb, 6, 0); /* mb_x */
        put_bits(&s->pb, 6, 0); /* mb_y */
        put_bits(&s->pb, 12, s->mb_width * s->mb_height);
    }

    put_bits(&s->pb, 3, 0);     /* ignored */
}

const enum PixelFormat rv10_encoder_formats[] = {PIX_FMT_YUV420P, PIX_FMT_NONE};

AVCodec rv10_encoder = {
#ifndef MSC_STRUCTS
    "rv10",
    AVMEDIA_TYPE_VIDEO,
    CODEC_ID_RV10,
    sizeof(MpegEncContext),
    MPV_encode_init,
    MPV_encode_picture,
    MPV_encode_end,
    .pix_fmts= (const enum PixelFormat[]){PIX_FMT_YUV420P, PIX_FMT_NONE},
    .long_name= NULL_IF_CONFIG_SMALL("RealVideo 1.0"),
#else
    /* name = */ "rv10",
    /* type = */ AVMEDIA_TYPE_VIDEO,
    /* id = */ CODEC_ID_RV10,
    /* priv_data_size = */ sizeof(MpegEncContext),
    /* init = */ MPV_encode_init,
    /* encode = */ MPV_encode_picture,
    /* close = */ MPV_encode_end,
    /* decode = */ 0,
    /* capabilities = */ 0,
    /* next = */ 0,
    /* flush = */ 0,
    /* supported_framerates = */ 0,
    /* pix_fmts = */ rv10_encoder_formats,
    /* long_name = */ NULL_IF_CONFIG_SMALL("RealVideo 1.0"),
    /* supported_samplerates = */ 0,
    /* sample_fmts = */ 0,
    /* channel_layouts = */ 0,
#endif
};
