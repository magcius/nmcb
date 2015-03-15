
# Create the tsnd from a wave file.

import sys
import wave

with open(sys.argv[1], 'rb') as wf:
    with open(sys.argv[2], 'w') as tf:
        w = wave.open(wf)
        h = """
/* This file was generated using mktsnd.py */

#include "tsnd.h"

struct tsnd _tsnd = {
    /* params */
    %s, %d, %d, %d, %d,

    /* data */
    {
""".strip()
        f = """    }
};
""".rstrip()

        sw = w.getsampwidth()
        if sw == 2:
            fmt = "SNDRV_PCM_FORMAT_S16_LE"
        elif sw == 1:
            fmt = "SNDRV_PCM_FORMAT_U8"
        else:
            yell

        rate = w.getframerate()
        nch = w.getnchannels()
        nf = w.getnframes()
        fs = nch * sw
        tf.write(h % (fmt, nch, rate, nf, fs))

        while True:
            b = w.readframes(6)
            if not b:
                break
            tf.write(''.join('0x%02x, ' % (ord(c),) for c in b))

        tf.write(f)
