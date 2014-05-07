#ifndef __duff_device_h__
#define __duff_device_h__

#define duff_device(N, to, from, count) switch(count % N) { case 0: do {*to++ = *from++; eight_case()} while (--n > 0);}
#define eight_case() case 7: *to++ = *from++;case 6: *to++ = *from++;case 5: *to++ = *from++;case 4: *to++ = *from++;case 3: *to++ = *from++;case 2: *to++ = *from++;case 1: *to++ = *from++;

#endif
