#include "bv.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BitVector {
    uint32_t length; //length of bits
    uint8_t *vector; //array of bytes
} BitVector;

BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) calloc(1, sizeof(BitVector));
    assert(v); //check if it created
    v->length = length;
    uint32_t val = length % 8 == 0 ? length / 8 : (length / 8) + 1;
    v->vector = calloc(val, sizeof(uint8_t));
    if (!v->vector) {
        free(v);
        v = NULL;
        return NULL;
    }
    return v;
}
void bv_delete(BitVector **v) {
    assert(*v);
    assert((*v)->vector);
    free((*v)->vector);
    free(*v);
    return;
}

uint32_t bv_length(BitVector *v) {
    assert(v);
    return v->length;
}

void bv_set_bit(BitVector *v, uint32_t i) {
    assert(v);
    //printf("%u ",(uint8_t) 0x1 << (i % 8));
    v->vector[i / 8] |= (1 << (i % 8)); //or to set
    return;
}

void bv_clr_bit(BitVector *v, uint32_t i) {
    assert(v);
    v->vector[i / 8] &= ~(1 << (i % 8)); //or to set
    return;
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    assert(v);
    return (v->vector[i / 8] & (1 << (i % 8))) >> (i % 8);
}

void bv_print(BitVector *v) {
    assert(v);
    for (uint32_t i = v->length; i > 0; i--) {
        printf("%u ", bv_get_bit(v, i));
    }
    printf("\n");
    return;
}
