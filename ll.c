#include "ll.h"

//extern uint64_t seeks; // Number of seeks performed.
//extern uint64_t links; // Number of links traversed.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
uint64_t seeks = 0;
uint64_t links = 0;

typedef struct LinkedList {
    uint32_t length;
    Node *head; //sentinel nodes
    Node *tail;
    bool mtf;
} LinkedList;

LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    ll->head = node_create(NULL, NULL); //sentinel node check later
    ll->tail = node_create(NULL, NULL);
    ll->head->next = ll->tail; //links them
    ll->tail->prev = ll->head; //links head and tails together
    ll->length = 0;
    ll->mtf = mtf;
    return ll;
}

void ll_delete(LinkedList **ll) {
    assert(ll); //check later
    assert(*ll);
    Node *curr = (*ll)->head->next;
    Node *temp;
    while (curr != (*ll)->tail) {
        temp = curr->next;
        node_delete(&curr);
        curr = temp;
    }
    node_delete(&((*ll)->tail));
    node_delete(&((*ll)->head));
    free(*ll);
    *ll = NULL;
    return;
}

uint32_t ll_length(LinkedList *ll) {
    assert(ll);
    return ll->length;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    assert(ll);
    seeks += 1; //
    for (Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
        links += 1;
        if (strcmp(curr->oldspeak, oldspeak) == 0) {
            if (ll->mtf) {
                //bridge over curr
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                //now move it back
                curr->next = ll->head->next;
                curr->prev = ll->head;
                ll->head->next = curr;
                curr->next->prev = curr;
            }
            return curr;
        }
    }
    return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    assert(ll);
    if (ll_lookup(ll, oldspeak) == NULL) {
        Node *n = node_create(oldspeak, newspeak);
        n->next = ll->head->next;
        n->prev = ll->head;
        ll->head->next = n;
        n->next->prev = n;
        ll->length += 1;
    }
    return;
}

void ll_print(LinkedList *ll) {
    assert(ll);
    for (Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
        node_print(curr);
    }
    return;
}
