#include "card.h"

Card* create_card(Column column, const char* description) {
    // allocazione memoria
    Card* new_card = (Card*)malloc(sizeof(Card));
    if (new_card == NULL) {
        perror("Errore allocazione memoria per struttura Card\n");
        return NULL;
    }

    // inizializza campi e ritorna puntatore
    new_card->id = next_id++;
    new_card->column = column;
    size_t size = (strlen(description) > MAX_CHARS) ? MAX_CHARS : strlen(description);
    strncpy(new_card->description, description, size-1);
    new_card->description[size] = '\0';
    new_card->user_id = -1;
    new_card->last_updated = time(NULL);
    new_card->next = NULL;
    
    return new_card;
}

void free_card(Card* card) {
    if (card != NULL) {
        free(card->description);
        free(card);
    }
}

Card* find_card(Card* card_list, int id) {
    for (Card* c = card_list; c != NULL; c = c->next)
        if (c->id == id)
            return c;
    
    return NULL;
}

void print_card(const Card* card) {
    if (card == NULL) {
        printf("Card NULL\n");
        return;
    }

    printf("Card ID: %d\n", card->id);
    printf("Description: %s\n", card->description);
    printf("Column: %s\n", card->column);
    printf("Last Updated: %s\n", ctime(&(card->last_updated)));
}