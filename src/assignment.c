#include "assignment.h"

#include <math.h>
#include <stddef.h>

unsigned bitset_symmetric_difference_count(uint32_t a, uint32_t b)
{
    uint32_t c = a ^ b;
    int sum = 0;
    for (int i = 0; i < 32; i++)
    {
        uint32_t t = c >> i;
        t = t & 1U;
        sum += t;
    }
    return sum;
}

uint32_t bit_range_mask(unsigned low, unsigned high)
{
    int width = (high - low) + 1;
    uint32_t mask = width == 32 ? 0xFFFFFFFF : (1U << width) - 1;
    return mask << low;
}

uint32_t splice_bits(
    uint32_t destination,
    uint32_t source,
    unsigned low,
    unsigned high)
{
    int width = (high - low) + 1;
    source <<= 32 - width;
    source >>= 32 - width;
    uint32_t mask = bit_range_mask(low, high);
    destination &= ~mask;
    source <<= low;
    destination |= source;
    return destination;
}

int32_t decode_twos_complement(uint32_t bits, unsigned width)
{
    int w = width;
    if (w == 0)
    {
        return 0;
    }
    bits <<= 32 - w;
    bits >>= 32 - w;
    uint32_t sign = bits >> (width - 1);
    if (!sign)
    {
        return bits;
    }
    int nBit = bits << (32 - w);
    nBit >>= 32 - w;
    return nBit;
}

bool add_signed_width(
    uint32_t a_bits,
    uint32_t b_bits,
    unsigned width,
    uint32_t *sum_bits)
{
    uint32_t mask = bit_range_mask(0, width - 1);
    a_bits &= mask;
    b_bits &= mask;
    short a_sign = a_bits >> (width - 1);
    short b_sign = b_bits >> (width - 1);
    *sum_bits = a_bits + b_bits;
    *sum_bits &= mask;
    short ans_sign = *sum_bits >> (width - 1);
    if ((a_sign == b_sign) && (a_sign != ans_sign))
    {
        return true;
    }
    return false;
}

int compare_signed_unsigned_math(
    int32_t signed_value,
    uint32_t unsigned_value)
{
    if (signed_value < 0)
    {
        return -1;
    }
    uint32_t ns = signed_value;
    if (ns > unsigned_value)
    {
        return 1;
    }
    if (ns == unsigned_value)
    {
        return 0;
    }
    return -1;
}

uint32_t multiply_by_24(uint32_t x)
{
    (void)x;
    return 0;
}

fp32_info_t fp32_decode(uint32_t bits)
{
    (void)bits;

    fp32_info_t info = {
        .negative = false,
        .kind = FP32_ZERO,
        .exponent_bits = 0,
        .fraction_bits = 0,
        .unbiased_exponent = -126,
    };
    return info;
}

double fp32_exact_value(uint32_t bits)
{
    (void)bits;
    return 0.0;
}

uint32_t fp32_next_up_bits(uint32_t bits)
{
    (void)bits;
    return 0;
}

bool fp32_truncate_to_int(uint32_t bits, int32_t *out)
{
    (void)bits;

    if (out != NULL)
    {
        *out = 0;
    }
    return false;
}
