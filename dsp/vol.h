#ifndef PICO_AMP_VOL_H
#define PICO_AMP_VOL_H

bool mute_l = false;
bool mute_r = false;

int32_t vol_mul_l = 0;
int32_t vol_mul_r = 0;

// 1 << 30
int32_t db_to_vol_hp[100] = {0x40000000,0x390a415f,0x32d64617,0x2d4efbd5,0x28619ae9,0x23fd6678,0x2013739e,0x1c9676c6,0x197a967f,0x16b54337,0x143d1362,0x1209a37a,0x10137987,0xe53ebb3,0xcc509ab,0xb618871,0xa24b062,0x90a4d2f,0x80e9f96,0x72e50a6,0x6666666,0x5b439bc,0x5156d68,0x487e5fb,0x409c2b0,0x399570c,0x3352529,0x2dbd8ad,0x28c423f,0x2455385,0x2061b89,0x1cdc38c,0x19b8c27,0x16ecac5,0x146e75d,0x1235a71,0x103ab3d,0xe76e1e,0xce4328,0xb7d4dd,0xa3d70a,0x9205c6,0x82248a,0x73fd65,0x676044,0x5c224e,0x521d50,0x492f44,0x4139d3,0x3a21f3,0x33cf8d,0x2e2d27,0x29279d,0x24ade0,0x20b0bc,0x1d22a4,0x19f786,0x17249c,0x14a050,0x126216,0x10624d,0xe9a2d,0xd03a7,0xb9956,0xa566d,0x936a1,0x83621,0x75186,0x685c8,0x5d031,0x52e5a,0x49e1d,0x41d8f,0x3aafc,0x344df,0x2e9dd,0x298c0,0x25076,0x21008,0x1d69b,0x1a36e,0x175d1,0x14d2a,0x128ef,0x108a4,0xebdc,0xd236,0xbb5a,0xa6fa,0x94d1,0x84a2,0x7636,0x695b,0x5de6,0x53af,0x4a96,0x4279,0x3b3f,0x34cd,0x2f0f};

/**
 * @brief Set the i2s mute.
 *
 * @param mute Mute state
 * @param ch Channel 0: Left & Right, 1: Left, 2: Right
 */
static void audio_set_mute(bool mute, int8_t ch)
{
    switch (ch)
    {
    case 0:
        mute_l = mute;
        mute_r = mute;
        break;
    case 1:
        mute_l = mute;
        break;
    case 2:
        mute_r = mute;
        break; 
    default:
        break;
    }
}

/**
 * @brief Changes the i2s volume.
 *
 * @param v Volume
 * @param ch Channel 0: Left & Right, 1: Left, 2: Right
 */
static void audio_set_volume(int16_t v, int8_t ch)
{
    int32_t vol_mul;

    int16_t pos = -v / 256;
    int16_t mix = -v % 256; // mix between two steps

    if (pos > 101)
        vol_mul = 0;
    else if (pos >= 100)
        vol_mul = (db_to_vol_hp[100] * (256 - mix)) >> 8;
    else
        vol_mul = (((int64_t)db_to_vol_hp[pos] * (256 - mix)) + ((int64_t)db_to_vol_hp[pos + 1] * mix)) >> 8;

    switch (ch)
    {
    case 0:
        vol_mul_l = vol_mul;
        vol_mul_r = vol_mul;
        break;
    case 1:
        vol_mul_l = vol_mul;
        break;
    case 2:
        vol_mul_r = vol_mul;
        break; 
    default:
        break;
    }
}

#endif