#include "ht.h"

#include "ll.h"
#include "speck.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
} HashTable;

HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        //Leviathan
        ht->salt[0] = 0x9846e4f157fe8840;
        ht->salt[1] = 0xc5f318d7e055afb8;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    assert(ht);
    assert(*ht);
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        if ((*ht)->lists[i] != NULL) {
            //checks if linked list exits then
            //deletes it
            ll_delete(&((*ht)->lists[i]));
        }
    }
    free((*ht)->lists);
    (*ht)->lists = NULL;
    free(*ht);
    *ht = NULL;
    return;
}

uint32_t ht_size(HashTable *ht) {
    assert(ht);
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    assert(ht);
    uint32_t index = hash(ht->salt, oldspeak) % ht->size;
    if (ht->lists[index] == NULL) {
        return NULL;
    }
    return ll_lookup(ht->lists[index], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    assert(ht);
    uint32_t index = hash(ht->salt, oldspeak) % ht->size; //to limit hash
    if (ht->lists[index] == NULL) {
        ht->lists[index] = ll_create(ht->mtf);
    }
    ll_insert(ht->lists[index], oldspeak, newspeak);

    return;
}

uint32_t ht_count(HashTable *ht) {
    assert(ht);
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->lists[i] != NULL) { //found a non null linkedlist
            count += 1;
        }
    }
    return count;
}

void ht_print(HashTable *ht) {
    assert(ht);
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->lists[i] != NULL) {
            ll_print(ht->lists[i]);
        }
    }
    return;
}
