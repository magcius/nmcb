
# Create the tsnd from a wave file.

import struct
import sys
import wave

with open(sys.argv[1], 'rb') as wf:
    with open(sys.argv[2], 'wb') as th:
        w = wave.open(wf)

        sw = w.getsampwidth()
        rate = w.getframerate()
        nch = w.getnchannels()
        nf = w.getnframes()
        fs = nch * sw
        th.write('TSND0000')
        th.write(struct.pack('<BBHHL', sw, nch, rate, fs, nf))

        while True:
            b = w.readframes(1024)
            if not b:
                break
            th.write(b)
