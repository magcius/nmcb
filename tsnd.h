/*
 * Copyright (C) 2015 Endless Mobile
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jasper St. Pierre <jstpierre@mecheye.net>
 */

/* A super simple data format that's included in the tsndplay
 * binary. You can generate one with mktsnd.py */

#pragma once

#include "asnd.h"
#include <string.h>
#include <stdbool.h>

#define TSND_MAGIC "TSND0000"

#pragma pack(push,1)

struct tsnd {
    char magic[8];
    uint8_t sample_width;
    uint8_t channels;
    uint16_t rate;
    uint16_t framesize;
    uint32_t nframes;
    uint8_t data[];
};

#pragma pack(pop)

static inline bool tsnd_is_valid(struct tsnd *tsnd) {
    return (strncmp (tsnd->magic, TSND_MAGIC, 8) == 0);
}

static inline snd_pcm_format_t tsnd_pcm_format(struct tsnd *tsnd) {
    switch (tsnd->sample_width) {
    case 1:
        return SNDRV_PCM_FORMAT_U8;
    default:
    case 2:
        return SNDRV_PCM_FORMAT_S16_LE;
    }
}
