#include "card.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Card* create_card(int id, const char* description, Column column) {
    // allocate memory for card object and check for malloc errors
    Card* new_card = (Card*)malloc(sizeof(Card));
    if (new_card == NULL) {
        perror("Malloc error in card_create: impossible to create a new card\n");
        return NULL;
    }

    // initialize card fields and return pointer to the new card
    new_card->id = id;
    strncpy(new_card->description, description, sizeof(new_card->description)-1);
    new_card->description[sizeof(new_card->description)-1] = '\0'; // ensure null-termination
    new_card->column = column;
    new_card->last_updated = time(NULL);
    return new_card;
}

void free_card(Card* card) {
    if (card != NULL) {
        free(card);
    }
}

void print_card(const Card* card) {
    if (card == NULL) {
        printf("Card is NULL\n");
        return;
    }

    printf("Card ID: %d\n", card->id);
    printf("Description: %s\n", card->description);
    printf("Column: %s\n", card->column);
    printf("Last Updated: %s\n", ctime(&(card->last_updated)));
}