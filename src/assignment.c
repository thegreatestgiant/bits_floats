#include "assignment.h"

#include <math.h>
#include <stddef.h>

unsigned bitset_symmetric_difference_count(uint32_t a, uint32_t b) {
    uint32_t c = a^b;
    uint32_t one = 1;
    int sum = 0;
    for(int i = 0; i<32;i++){
        uint32_t t = c >>i;
        t = t & one;
        sum+=t;
    }
    return sum;
}

uint32_t bit_range_mask(unsigned low, unsigned high) {
    (void)low;
    (void)high;
    return 0;
}

uint32_t splice_bits(
    uint32_t destination,
    uint32_t source,
    unsigned low,
    unsigned high
) {
    (void)destination;
    (void)source;
    (void)low;
    (void)high;
    return 0;
}

int32_t decode_twos_complement(uint32_t bits, unsigned width) {
    (void)bits;
    (void)width;
    return 0;
}

bool add_signed_width(
    uint32_t a_bits,
    uint32_t b_bits,
    unsigned width,
    uint32_t *sum_bits
) {
    (void)a_bits;
    (void)b_bits;
    (void)width;

    if (sum_bits != NULL) {
        *sum_bits = 0;
    }
    return false;
}

int compare_signed_unsigned_math(
    int32_t signed_value,
    uint32_t unsigned_value
) {
    (void)signed_value;
    (void)unsigned_value;
    return 0;
}

uint32_t multiply_by_24(uint32_t x) {
    (void)x;
    return 0;
}

fp32_info_t fp32_decode(uint32_t bits) {
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

double fp32_exact_value(uint32_t bits) {
    (void)bits;
    return 0.0;
}

uint32_t fp32_next_up_bits(uint32_t bits) {
    (void)bits;
    return 0;
}

bool fp32_truncate_to_int(uint32_t bits, int32_t *out) {
    (void)bits;

    if (out != NULL) {
        *out = 0;
    }
    return false;
}
