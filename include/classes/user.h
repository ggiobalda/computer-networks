/**
 * @file user.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definizione struct User e relative funzioni
 * 
 */
#ifndef USER_H
#define USER_H

#include "../network/utils.h"

/**
 * @brief Utente nel kanban
 * 
 * @param id Numero di porta
 * @param current_card_id ID della carta attualmente in gestione
 * @param socket Descrittore del socket
 * @param next Puntatore al prossimo utente in lista
 * 
 */
typedef struct User{
    int id;
    int current_card_id;
    int socket;
    struct User* next;
} User;


/**
 * @brief Costruttore utente
 * 
 * @param port Porta, usata come ID dell'utente
 * @param socket Descrittore del socket dell'utente
 * 
 * @return Puntatore all'utente appena creato, NULL su errore
 * 
 */
User* create_user(int port, int socket);

/**
 * @brief Distruttore utente
 * 
 * @param user Puntatore all'utente
 * 
 */
void free_user(User* user);

/**
 * @brief Trova utente in base alla porta
 *
 * @param user_list Rferimento alla lista degli utenti
 * @param port Porta, usata come ID dell'utente
 * 
 * @return Puntatore all'utente, NULL se non presente in lista
 */
User* find_user_by_port(User* user_list, int port);

/**
 * @brief Trova utente in base al socket
 *
 * @param user_list rRferimento alla lista degli utenti
 * @param socket Descrittore socket dell'utente
 * 
 * @return Puntatore all'utente, NULL se non presente in lista
 */
User* find_user_by_socket(User* user_list, int socket);

#endif