/**
 * @file card.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definitions for the Card structure and related functions
 * 
 */

#ifndef CARD_H
#define CARD_H

#include <time.h>

/**
 * @brief Columns in the Kanban board
 *
 */
typedef enum Column {
    TODO = 0,
    DOING = 1,
    DONE = 2
} Column;

/**
 * @brief Task in the Kanban board
 * 
 * @param id Unique identifier for the card
 * @param column Current column of the card in Kanban board
 * @param description Detailed description of the card
 * @param last_updated Timestamp of the last card's update
 * 
 */
typedef struct Card{
    int id;
    char description[255];
    Column column;
    time_t last_updated;
    struct Card* next;
} Card;

/**
 * @brief Create a new card object
 * 
 * @param id New card's id, must be unique
 * @param description Pointer to buffer containing card's description
 * @param column Kanban column where the new card will be placed
 * @return Card* - Pointer to newly created card, NULL on error
 * 
 */
Card* create_card(int id, const char* description, Column column);

/**
 * @brief Free memory allocated for object card
 * 
 * @param card Pointer to card
 * 
 */
void free_card(Card* card);

/**
 * @brief Print card details ina standard format
 * 
 * @param card Pointer to card
 * 
 */
void print_card(const Card* card);

#endif