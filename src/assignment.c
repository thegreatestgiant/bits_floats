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
    return (x << 5) - (x << 3);
}

fp32_info_t fp32_decode(uint32_t bits)
{
    uint32_t eMask = bit_range_mask(23, 30);
    uint32_t fMask = bit_range_mask(0, 22);
    uint32_t exp_raw = bits & eMask;
    exp_raw >>= 23;
    uint8_t exp = exp_raw;
    uint32_t fraction = bits & fMask;
    fp32_kind_t kind;
    int ub;
    if (exp == 0)
    {
        ub = -126;
        if (fraction == 0)
        {
            kind = FP32_ZERO;
        }
        else
        {
            kind = FP32_SUBNORMAL;
        }
    }
    else if (exp == 255)
    {
        ub = 0;
        if (fraction == 0)
        {
            kind = FP32_INFINITY;
        }
        else
        {
            kind = FP32_NAN;
        }
    }
    else
    {
        ub = exp - 127;
        kind = FP32_NORMAL;
    }
    fp32_info_t info = {
        .negative = bits >> 31,
        .kind = kind,
        .exponent_bits = exp,
        .fraction_bits = fraction,
        .unbiased_exponent = ub,
    };
    return info;
}

double fp32_exact_value(uint32_t bits)
{
    fp32_info_t info = fp32_decode(bits);
    if (info.kind == FP32_ZERO)
    {
        if (info.negative)
        {
            return -0.0;
        }
        return 0.0;
    }
    if (info.kind == FP32_INFINITY)
    {
        if (info.negative)
        {
            return -INFINITY;
        }
        return INFINITY;
    }
    if (info.kind == FP32_NAN)
    {
        return NAN;
    }
    double M = (double)info.fraction_bits / (double)(1 << 23);
    short n = info.negative ? -1 : 1;
    if (info.kind == FP32_NORMAL)
    {
        return n * (1 + M) * ldexp(1.0, info.unbiased_exponent);
    }
    return n * (0 + M) * ldexp(1.0, info.unbiased_exponent);
}

uint32_t fp32_next_up_bits(uint32_t bits)
{
    fp32_info_t info = fp32_decode(bits);
    // double val = fp32_exact_value(bits);
    if (info.kind == FP32_ZERO)
    {
        return 1;
    }
    if (info.kind == FP32_NAN || (info.kind == FP32_INFINITY && !info.negative))
    {
        return bits;
    }
    if (info.kind == FP32_INFINITY)
    {
        return 0xFF7FFFFF;
    }
    if (!info.negative)
    {
        return bits + 1;
    }
    else
    {
        return bits - 1;
    }
}

bool fp32_truncate_to_int(uint32_t bits, int32_t *out)
{
    fp32_info_t info = fp32_decode(bits);
    if (info.kind == FP32_INFINITY || info.kind == FP32_NAN || bits == 0x4F000000)
    {
        return false;
    }
    if (info.kind == FP32_ZERO)
    {
        *out = 0;
        return true;
    }
    if (info.unbiased_exponent < 0)
    {
        *out = 0;
        return true;
    }
    uint32_t M = info.fraction_bits;
    if (info.kind == FP32_NORMAL)
    {
        M |= bit_range_mask(23, 23);
    }
    if (info.unbiased_exponent < 23)
    {
        M >>= (23 - info.unbiased_exponent);
        *out = M;
        if (info.negative)
        {
            *out = -M;
        }
        return true;
    }
    if (bits == 0x4EFFFFFF)
    {
        *out = 2147483520;
        return true;
    }
    if (bits == 0xCF000000)
    {
        *out = -2147483648;
        return true;
    }
    if (info.unbiased_exponent >= 31)
    {
        return false;
    }
    M <<= (info.unbiased_exponent - 23);

    if (M >= INT32_MAX)
    {
        return false;
    }
    *out = M;
    if (info.negative)
    {
        *out = -M;
    }
    return true;
}
