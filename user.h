/**
 * @file user.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Definizione struct User e relative funzioni
 * 
 */
#ifndef USER_H
#define USER_H

#include "utils.h"

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
 * @brief 
 * 
 * @param users lista degli utenti della lavagna
 * @param usr id del nuovo utente
 * @param descp descrittore del socket dell'utente
 * 
 * @return ritorna 0 se ha successo, -1 altrimenti
 */
//int insert_User_in_list(User** users, int usr, int descp);

/**
 * @brief La funzione estrae un'utente dalla lista
 * 
 * La funzione scorre la lista per cercare l'utente usr, se lo trova lo elimina
 * 
 * @param usr id dell'utente da estrarre
 * 
 * @return la funzione ritorna 0 se l'utente è stato trovato, -1 altrimenti
 */
//int extract_User(User** user_list, int usr);

/**
 * @brief La funzione ottiene il riferimento ad un'utente tramite il suo id
 *
 * @param top riferimento alla lista degli utenti
 * @param port porta dell'utente da ottenere
 * 
 * @return la funzione ritorna il riferimento all'utente se esiste, altrimenti NULL
 */
//User* get_User_by_port(User* top, int port);

/**
 * @brief La funzione ottiene il riferimento ad un'utente tramite il suo socket
 *
 * @param top riferimento alla lista degli utenti
 * @param port socket dell'utente
 * 
 * @return la funzione ritorna il riferimento all'utente se esiste, altrimenti NULL
 */
//User* get_User_by_socket(User* top, int sock);

/**
 * @brief Ottiene la porta dell'utente
 * 
 * @param user utente
 * 
 * @return porta dell'utente
 */
//int get_User_port(User* user);

/**
 * @brief Ottiene lo stato relativo all'utente
 * 
 * @param user utente 
 * 
 * @return stato dell'utente
 */
//User_card_status get_Usertatus(User* user);

/**
 * @brief Ottiene la card relativa all'utente
 * 
 * @param user utente 
 * 
 * @return id della card dell'utente
 */
//int get_User_card(User* user);

/**
 * @brief Ottiene il descrittore del socket dell'utente
 * 
 * @param user utente 
 * 
 * @return descrittore del socket
 */
//int get_Userocket(User* user);

/**
 * @brief La funzione permette di modificare la card dell'utente
 * 
 * La funzione assegna all'utente nella lista la card ad esso assegnata
 * 
 * @param user utente a cui modificare lo stato
 * @param card_id id della card
 */
//void set_User_card(User* user, int card_id);

/**
 * @brief La funzione permette di modificare lo stato dell'utente
 * 
 * La funzione assegna all'utente nella lista lo stato in cui si deve trovare
 * 
 * @param user utente a cui modificare lo stato
 * @param status stato che deve essere modificato
 */
//void set_Usertatus(User* user, User_card_status status);

/**
 * @brief funzione utilizzata per ottenere un array contenente tutti gli utenti, apparte quello corrente
 * 
 * @param top riferimento alla lista di utenti
 * @param users lista da modificare
 * @param users_number numero di utenti che conta la lista
 * @param current_user utente corrente
 */
//void get_Users(User* top, int users[], int users_number, int current_user);

/**
 * @brief debug print function
 */
//void prova_print(User *user_list);

#endif