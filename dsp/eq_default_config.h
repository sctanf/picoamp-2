#ifndef PICO_AMP_EQ_DEFAULT_CONFIG_H
#define PICO_AMP_EQ_DEFAULT_CONFIG_H

#ifndef EQ_BASS
#define EQ_BASS 1.0103039413192074,-1.9969618889340095,0.9867280269044961,-1.9969618889340095,0.9970319682237037 // PK Fc 64 Hz Gain 18 dB Q 1
#endif

#ifndef LIMIT_MUL
#define LIMIT_MUL ((dspfx)(0.316*(double)(1<<27))) // 0.32x > -10dB limit on output, -10-18 > -28dB limit on input
#endif

#ifndef BASS_MUL
#define BASS_MUL floatfx(1./7.9) // Bass peak filter is 18dB > 8x
#endif

#ifndef EQ_I_0
#define EQ_I_0 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_1
#define EQ_I_1 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_3
#define EQ_I_3 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_5
#ifdef EQ_I_4
#define LAST_EQ_BUF buf1
#else
#define EQ_I_5 1.0,0.0,0.0,0.0,0.0
#endif
#endif

#ifndef EQ_I_7
#define EQ_I_7 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_9
#define EQ_I_9 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_11
#define EQ_I_11 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_13
#define EQ_I_13 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_15
#define EQ_I_15 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef EQ_I_17
#define EQ_I_17 1.0,0.0,0.0,0.0,0.0
#endif

#ifndef LAST_EQ_BUF
#define LAST_EQ_BUF buf0
#endif

#endif