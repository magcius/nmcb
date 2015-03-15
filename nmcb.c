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

/* nmcb is a super simple sound API that uses ALSA device nodes
 * directly, meant for really embedded scenarios like early boot. */

#include "nmcb.h"

#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

static void clr_mask (struct snd_mask *m)        { memset (m, 0x00, sizeof (*m)); }
static void any_mask (struct snd_mask *m)        { memset (m, 0xFF, sizeof (*m)); }
static void set_mask (struct snd_mask *m, int v) { clr_mask (m); m->bits[(v >> 5)] = (1 << v); }

static void set_mask_param (struct snd_pcm_hw_params *hwparams, int var, int val) {
    set_mask (&hwparams->masks[var - SNDRV_PCM_HW_PARAM_FIRST_MASK], val);
    hwparams->cmask |= (1 << var);
}

static void any_itrv (struct snd_interval *i) { *i = (struct snd_interval) { .max = -1 }; }
static void set_itrv (struct snd_interval *i, int v) { *i = (struct snd_interval) { .min = v, .max = v, .integer = 1 }; }

static void set_itrv_param (struct snd_pcm_hw_params *hwparams, int var, int val) {
    set_itrv (&hwparams->intervals[var - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL], val);
    hwparams->cmask |= (1 << var);
}

int
nmcb_set_params (int fd,
                 snd_pcm_format_t format,
                 snd_pcm_access_t access,
                 int channels, unsigned rate, unsigned latency)
{
    struct snd_pcm_hw_params hwparams = { 0 };
    int k;

    for (k = SNDRV_PCM_HW_PARAM_FIRST_MASK; k <= SNDRV_PCM_HW_PARAM_LAST_MASK; k++)
        any_mask (&hwparams.masks[k - SNDRV_PCM_HW_PARAM_FIRST_MASK]);
    for (k = SNDRV_PCM_HW_PARAM_FIRST_INTERVAL; k <= SNDRV_PCM_HW_PARAM_LAST_INTERVAL; k++)
        any_itrv (&hwparams.intervals[k - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL]);

    hwparams.flags = SNDRV_PCM_HW_PARAMS_NORESAMPLE;
    hwparams.rmask = ~0U;
    hwparams.cmask = 0;
    hwparams.info = ~0U;

    set_mask_param (&hwparams, SNDRV_PCM_HW_PARAM_ACCESS, access);
    set_mask_param (&hwparams, SNDRV_PCM_HW_PARAM_FORMAT, format);
    set_itrv_param (&hwparams, SNDRV_PCM_HW_PARAM_CHANNELS, channels);
    set_itrv_param (&hwparams, SNDRV_PCM_HW_PARAM_RATE, rate);
    set_itrv_param (&hwparams, SNDRV_PCM_HW_PARAM_BUFFER_TIME, latency);

    if (ioctl (fd, SNDRV_PCM_IOCTL_HW_PARAMS, &hwparams) < 0)
        return -1;

    return 0;
}

int
nmcb_prepare (int fd)
{
    return ioctl (fd, SNDRV_PCM_IOCTL_PREPARE, 0);
}

int
nmcb_write_data (int fd, void *buf, int nframes)
{
    struct snd_xferi xferi = {
        .buf = buf,
        .frames = nframes
    };
    if (ioctl (fd, SNDRV_PCM_IOCTL_WRITEI_FRAMES, &xferi) < 0)
        return -1;
    return xferi.result;
}

int
nmcb_open (void)
{
    return open ("/dev/snd/pcmC0D0p", O_RDWR);
}
