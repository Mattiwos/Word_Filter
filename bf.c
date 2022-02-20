#include "bf.h"

#include "speck.h" //contains hash()

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BloomFilter {
    uint64_t primary[2]; //primary hash function salt
    uint64_t secondary[2]; //Secondary hash function salt
    uint64_t tertiary[2]; // Tertiary Hash function salt
    BitVector *filter;
} BloomFilter;

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = 0x5adf08ae86d36f21;
        bf->primary[1] = 0xa267bbd3116f3957;

        bf->secondary[0] = 0x419d292ea2ffd49e;
        bf->secondary[1] = 0x09601433057d5786;

        bf->tertiary[0] = 0x50d8bb08de3818df;
        bf->tertiary[1] = 0x4deaae187c16ae1d;

        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
    assert(bf);
    assert(*bf);
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
}

uint32_t bf_size(BloomFilter *bf) {
    assert(bf);
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t key1 = hash(bf->primary, oldspeak) % bf_size(bf); //not sure what you mod it with
    uint32_t key2 = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t key3 = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, key1);
    bv_set_bit(bf->filter, key2);
    bv_set_bit(bf->filter, key3);
    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t key1 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t key2 = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t key3 = hash(bf->tertiary, oldspeak) % bf_size(bf);
    if (bv_get_bit(bf->filter, key1) && bv_get_bit(bf->filter, key2)
        && bv_get_bit(bf->filter, key3)) {
        return true;
    }
    return false;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    assert(bf);
    bv_print(bf->filter);
}
