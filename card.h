/**
 * @file card.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definizione della struct Card e relative funzioni
 * 
 */
#ifndef CARD_H
#define CARD_H

#include "utils.h"

/**
 * @brief Task nel kanban
 * 
 * @param id ID univoco
 * @param column Colonna corrente in cui si trova
 * @param description Descrizione dettagliata della card
 * @param user ID dell'utente che implementa la card
 * @param last_updated Timestamp dell'ultima modifica
 * @param next Puntatore alla prossima card in lista
 * 
 */

typedef struct Card {
    int id;
    Column column;
    char* description;
    int user_id;
    time_t last_updated;
    struct Card* next;
} Card;

/**
 * @brief Costruttore card
 * 
 * @param id ID della card
 * @param column Colonna in cui collocare la carta
 * @param description Puntatore al buffer contenente descrizione
 * 
 * @return Puntatore alla card creata, NULL su errore
 * 
 */
Card* create_card(int id, Column column, const char* description);

/**
 * @brief Distruttore card
 * 
 * @param card Puntatore alla card
 * 
 */
void free_card(Card* card);

/**
 * @brief Trova card in base all'id
 * 
 * @param card_list Puntatore alla lista di card
 * @param id Id della card da trovare
 * 
 * @return Puntatore alla card, NULL se non esiste
 * 
 */
Card* find_card(Card* card_list, int id);

/**
 * @brief Stampa una Card
 * 
 * @param card Puntatore alla card da stampare
 * 
 */
void print_card(const Card* card);


#endif