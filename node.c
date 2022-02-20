#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    assert(n);
    //From slack might have to fix later
    if (oldspeak != NULL) {
        n->oldspeak = malloc(strlen(oldspeak) + 1);
        assert(n->oldspeak);
        strcpy(n->oldspeak, oldspeak);
    } else {
        n->oldspeak = NULL;
    }
    if (newspeak != NULL) {
        n->newspeak = malloc(strlen(newspeak) + 1);
        assert(n->newspeak);
        strcpy(n->newspeak, newspeak);
    } else {
        n->newspeak = NULL;
    }
    n->next = n->prev = NULL;
    return n;
}

void node_delete(Node **n) {
    assert(n);
    assert(*n);
    if ((*n)->oldspeak) {
        free((*n)->oldspeak);
        (*n)->oldspeak = NULL;
    }
    if ((*n)->newspeak) {
        free((*n)->newspeak);
        (*n)->newspeak = NULL;
    }
    free(*n);
    *n = NULL;
    return;
}

void node_print(Node *n) {
    assert(n);
    if (n->oldspeak != NULL && n->newspeak != NULL) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    } else if (n->oldspeak != NULL) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
