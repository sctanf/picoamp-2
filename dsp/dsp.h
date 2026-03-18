#ifndef PICO_AMP_DSP_H
#define PICO_AMP_DSP_H

#include <stdio.h>

#include "pico/stdlib.h"

typedef int32_t dspfx;

#define fpformat 15
#define mulfx(a,b) ((dspfx)(((int64_t)(a)*(int64_t)(b))>>fpformat))
#define intfx(a) ((a)<<fpformat)
#define fxint(a) ((a)>>fpformat)
#define floatfx(a) ((dspfx)((a)*(1<<fpformat)))
#define fxfloat(a) ((float)(a)/(1<<fpformat))
#define fxabs(a) ((a)<0?-(a):(a))

#define fpformat2 30
#define mulfx2(a,b) ((dspfx)(((int64_t)(a)*(int64_t)(b))>>fpformat2))
#define intfx2(a) ((a)<<fpformat2)
#define fxint2(a) ((a)>>fpformat2)
#define floatfx2(a) ((dspfx)((a)*(1<<fpformat2)))
#define fxfloat2(a) ((float)(a)/(1<<fpformat2))
#define fxabs2(a) ((a)<0?-(a):(a))

#define fpformat3 28
//#define mulfx0(a,b) ((int64_t)(a)*(int64_t)(b))
#define mulfx3(a,b) ((dspfx)(((int64_t)(a)*(int64_t)(b))>>fpformat3))
#define fxint3(a) ((a)>>fpformat3)
//#define mulshift(a) ((dspfx)fxint3(a))
#define floatfx3(a) ((int64_t)((a)*(1<<fpformat3)))

// https://github.com/ploopyco/headphones/blob/master/firmware/code/fix16.inl
#define mulfxHL(bH, bL, a) ((a >> 14) * bH + (((a >> 14) * bL + bH * (a & 0x3FFF)) >> 14))

typedef struct {
	dspfx a1z, a2z, b1z, b2z;
	dspfx a1zr, a2zr, b1zr, b2zr;
} biquad;

#define biquad(a) biquad a = {.a1z=0,.a2z=0,.b1z=0,.b2z=0,.a1zr=0,.a2zr=0,.b1zr=0,.b2zr=0};
#define biqconstfxHL(a) (floatfx3(a) >> 14),(floatfx3(a) & 0x3FFF)
#define biquadconstfx(a, b, c, d, e) biqconstfxHL(a), biqconstfxHL(b), biqconstfxHL(c), biqconstfxHL(d), biqconstfxHL(e)
#define biquadconstsfx(a) biquadconstfx(a)

static inline void process_biquad(biquad *const filter, int32_t a0H, int32_t a0L, int32_t a1H, int32_t a1L, int32_t a2H, int32_t a2L, int32_t b1H, int32_t b1L, int32_t b2H, int32_t b2L, int16_t iters, int32_t *in, int32_t *out) {
	int16_t iters2 = iters * 2;
	out[0] = mulfxHL(a0H, a0L, in[0]) + mulfxHL(a1H, a1L, filter->a1z)  - mulfxHL(b1H, b1L, filter->b1z)  + mulfxHL(a2H, a2L, filter->a2z)  - mulfxHL(b2H, b2L, filter->b2z);
	out[2] = mulfxHL(a0H, a0L, in[2]) + mulfxHL(a1H, a1L, in[0])        - mulfxHL(b1H, b1L, out[0])       + mulfxHL(a2H, a2L, filter->a1z)  - mulfxHL(b2H, b2L, filter->b1z);
	out[1] = mulfxHL(a0H, a0L, in[1]) + mulfxHL(a1H, a1L, filter->a1zr) - mulfxHL(b1H, b1L, filter->b1zr) + mulfxHL(a2H, a2L, filter->a2zr) - mulfxHL(b2H, b2L, filter->b2zr);
	out[3] = mulfxHL(a0H, a0L, in[3]) + mulfxHL(a1H, a1L, in[1])        - mulfxHL(b1H, b1L, out[1])       + mulfxHL(a2H, a2L, filter->a1zr) - mulfxHL(b2H, b2L, filter->b1zr);
	for (int i = 4; i < iters2; i++) {
		out[i] = mulfxHL(a0H, a0L, in[i]) + mulfxHL(a1H, a1L, in[i - 2]) - mulfxHL(b1H, b1L, out[i - 2]) + mulfxHL(a2H, a2L, in[i - 4]) - mulfxHL(b2H, b2L, out[i - 4]); // takes up the most time by far..
	}
	filter->a2z = in[iters2 - 4];
	filter->b2z = out[iters2 - 4];
	filter->a1z = in[iters2 - 2];
	filter->b1z = out[iters2 - 2];
	filter->a2zr = in[iters2 - 3];
	filter->b2zr = out[iters2 - 3];
	filter->a1zr = in[iters2 - 1];
	filter->b1zr = out[iters2 - 1];
}

#endif