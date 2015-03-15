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

/* A simple sound player using nmcb.c to play a sound built into the
 * binary, generated with mktsnd.py. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "nmcb.h"
#include "tsnd.h"

/* This is provded by tsnddata.o */
extern struct tsnd _tsnd;

int main (int argc, char *argv[]) {
    int ret = EXIT_FAILURE;
    int fd;
    int frames;

    fd = nmcb_open ();
    if (fd < 0)
        goto out;

    if (nmcb_set_params (fd,
                         _tsnd.format,
                         SNDRV_PCM_ACCESS_RW_INTERLEAVED,
                         _tsnd.channels,
                         _tsnd.rate,
                         500000) < 0)
        goto out;

    if (nmcb_prepare (fd) < 0)
        goto out;

    frames = nmcb_write_data (fd, &_tsnd.data, _tsnd.nframes);
    if (frames != _tsnd.nframes) {
        fprintf (stderr, "not all frames written\n");
        goto out;
    }

    ret = EXIT_SUCCESS;

 out:
    if (fd >= 0)
        close (fd);
    return ret;
}
