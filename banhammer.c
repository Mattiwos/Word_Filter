#include "bf.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "parser.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //need for strdup
#include <sys/stat.h> //used to set permision
#include <sys/types.h>
#include <unistd.h> //for getop()

#define OPTIONS "ht:f:ms" //options for input
#define WORD    "[A-Za-z0-9]+(('|-|_)[A-Za-z0-9]+)*"

int main(int argc, char **argv) {
    int opt;
    regex_t re;
    uint32_t hashsize = 10000; //default hash size
    uint32_t bloomsize = 1 << 20; //fix later 2 ^ 20
    bool mtf = false; //move to the front default
    bool stats = false; //prints statistics default

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //program argument parser.
        switch (opt) {
        case 'h':
            //print message
            fprintf(stderr,
                "SYNOPSIS\n"
                "  A word filtering program for the GPRSC.\n"
                "  Filters out and reports bad words parsed from stdin.\n"
                "\n"
                "USAGE\n"
                "  %s [-hsm] [-t size] [-f size]\n"
                "\n"
                "OPTIONS\n"
                "  -h           Program usage and help.\n"
                "  -s           Print program statistics.\n"
                "  -m           Enable move-to-front rule.\n"
                "  -t size      Specify hash table size (default: 10000).\n"
                "  -f size      Specify Bloom filter size (default: 2^20).\n",
                argv[0]);
            exit(1);
            break;
        case 't': hashsize = atoi(optarg); break;
        case 'f': bloomsize = atoi(optarg); break;
        case 'm': mtf = true; break;
        case 's': stats = true; break;
        }
    }
    //check if reg works
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    BloomFilter *bf = bf_create(bloomsize); //inilized BloomFilter
    HashTable *ht = ht_create(hashsize, mtf); //inilized HashTable;

    //Phase 1
    //Add words from badspeak.tst to BF and HT
    //
    //Add words from newspeak.txt to BF and HT
    //
    FILE *badlist = fopen("badspeak.txt", "r");

    char badword[4096]; //one block
    while (fscanf(badlist, "%s\n", badword) != EOF) {
        bf_insert(bf, badword);
        ht_insert(ht, badword, NULL);
    }
    fclose(badlist);

    FILE *newspeak = fopen("newspeak.txt", "r");
    char oldword[4096];
    char newword[4096];
    while (fscanf(newspeak, "%s %s\n", oldword, newword) != EOF) {
        bf_insert(bf, oldword);
        ht_insert(ht, oldword, newword);
    }
    fclose(newspeak);
    //Phase 2 (firewall)
    //read in user inputo
    //stdin
    //iniz ll bad word
    //iniz ll couldbe fixed words
    //for each word
    //	if in BF
    //	    check hash table
    //	else
    //	    not a bad word
    char *word = NULL;
    LinkedList *badspeak = ll_create(mtf); //linked list of bad speak
    LinkedList *rightspeak = ll_create(mtf);
    while ((word = next_word(stdin, &re)) != NULL) {
        for (int i = 0; i < 4096 && word[i] != '\0';
             i++) { //lower case all the letters in teh buffer
            word[i] = tolower(word[i]); //lower chars
        }
        if (bf_probe(bf, word)) { //checks if the bloom filter returns a postive
            Node *n = ht_lookup(ht, word); //looks up the work in hash table
            if (n == NULL) { //false postive
                continue;
            } else if (n->newspeak == NULL) { //doesn't have new word translation
                ll_insert(badspeak, n->oldspeak, NULL);
            } else if (n->newspeak != NULL && n->oldspeak != NULL) {
                ll_insert(rightspeak, n->oldspeak, n->newspeak);
            }
        }
    }
    if (stats) { //prints all the stats
        fprintf(stdout, "Seeks: %lu\n", seeks);
        fprintf(stdout, "Avg seek length: %.6f\n", (double) links / seeks);
        fprintf(stdout, "Hash table load: %.6f%%\n", 100 * ((double) ht_count(ht) / ht_size(ht)));
        fprintf(stdout, "Bloom filter load: %.6f%%\n", 100 * ((double) bf_count(bf) / bf_size(bf)));

        //close the program
        clear_words();
        regfree(&re);
        ht_delete(&ht); //delete hash table and bloom filter
        bf_delete(&bf);
        ll_delete(&badspeak);
        ll_delete(&rightspeak);
        return 0;
    }

    //prints messages
    if (ll_length(badspeak) > 0 && ll_length(rightspeak) > 0) {
        //contains bad speak and right speak
        printf("%s", mixspeak_message);
        ll_print(badspeak);
        ll_print(rightspeak);

    } else if (ll_length(badspeak) > 0) {
        //contains only bad speak in std in
        printf("%s", badspeak_message);
        ll_print(badspeak);
    } else if (ll_length(rightspeak) > 0) {
        //contains only right speak in stdin
        printf("%s", goodspeak_message);
        ll_print(rightspeak);
    }

    //close program
    clear_words();
    regfree(&re);
    ht_delete(&ht); //delete hash table and bloom filter
    bf_delete(&bf);
    ll_delete(&badspeak);
    ll_delete(&rightspeak);
    return 0;
}
