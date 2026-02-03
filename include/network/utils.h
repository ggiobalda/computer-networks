/**
 * @file utils.h
 * @author Giorgio Baldacci (g.baldacci9@studenti.unipi.it)
 * @brief Costanti utili per il progetto
 * 
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <time.h>

// costanti
#define BOARD_PORT 5678
#define BOARD_ADDRESS "127.0.0.1"
#define MIN_PORT 5679
#define MAX_USERS 20
#define MAX_CARD_DESC_CHARS 30
#define N_COLUMNS 3
#define MAX_PAYLOAD_SIZE 1024
#define PING_INTERVAL 90
#define PONG_INTERVAL 30

/**
 * @brief Colonne nella lavagna
 *
 */
typedef enum Column {
    TODO,
    DOING,
    DONE
} Column;

extern const char* ColumnNames[];

/**
 * @brief Tipo di messaggio inviato
 * 
 * Suddivisi per ruoli di sender/receiver (Board to User, User to Board, User to User)
 * 
 */
typedef enum {
    BtU_AVAILABLE_CARD,
    BtU_PING,
    BtU_SHOW_LAVAGNA,
    BtU_SEND_USER_LIST,

    UtB_HELLO,
    UtB_QUIT,
    UtB_SHOW_LAVAGNA,
    UtB_SEND_USER_LIST,
    UtB_PONG,
    UtB_CREATE_CARD,
    UtB_ACK_CARD,
    UtB_CARD_DONE,
    
    UtU_COST
} MsgType;

/**
 * @brief Header contenente tipo e dimensione del messaggio
 * 
 */
typedef struct {
    MsgType type;
    int payload_len;
} MsgHeader;

/**
 * @brief Payload per il messaggio UtB_HELLO
 * 
 * @param port Porta usata dall'utente per comunicare con kanban
 */
typedef struct {
    int port;
} MsgHelloPayload;

/**
 * @brief Payload inviato dalla lavagna a tutti gli utenti per avviare l'asta
 * 
 * @param card_id ID della card oggetto dell'asta 
 * @param description Descrizione della card
 * @param n_users Numero di utenti partecipanti all'asta
 * @param users_ports Array di porte dei partecipanti
 * 
 */
typedef struct {
    int card_id;
    char description[MAX_CARD_DESC_CHARS];
    int n_users;
    int users_ports[MAX_USERS];
} MsgAvailableCardPayload;

/**
 * @brief Payload per inviare lavagna sotto forma di stringa
 * 
 * @param string Buffer contenente payload
 * 
 */
typedef struct {
    char string[MAX_PAYLOAD_SIZE];
} MsgShowLavagnaPayload;

/**
 * @brief Payload per inviare lista di utenti connessi
 * 
 * @param n_users Numero utenti
 * @param users_ports Lista utenti
 * 
 */
typedef struct {
    int n_users;
    int users_ports[MAX_USERS];
} MsgUserListPayload;

/**
 * @brief Payload inviato da ogni utente nell'asta a tutti gli altri
 * 
 * @param card_id ID della card oggetto dell'asta 
 * @param cost Costo associato alla card per il sender
 * @param sender_port Porta associata all'utente sender
 * 
 */
typedef struct {
    int card_id;
    int cost;
    int sender_port;
} MsgChooseUserPayload;

/**
 * @brief Invia un messaggio tramite socket
 * 
 * @param socket Valore del socket su cui inviare messaggio
 * @param type Tipo di messaggio
 * @param payload Puntatore a dove leggere corpo del messaggio
 * @param payload_len Dimensione del corpo del messaggio
 
 * @return 1 se ha successo, error value altrimenti
 */
int send_msg(int socket, MsgType type, const void* payload, int payload_len);

/**
 * @brief Riceve un messaggio tramite socket
 * 
 * @param socket Valore del socket su cui ricevere messaggio
 * @param head Puntatore a struct MsgHead dove scrivere info sul messaggio
 * @param payload Puntatore a buffer dove scrivere corpo del messaggio
 * @param MAX_PAYLOAD_SIZE_len Dimensione massima del payload
 *
 * @return 1 se ha successo, error value altrimenti
 */
int recv_msg(int socket, MsgHeader* head, void* payload, int MAX_PAYLOAD_SIZE_len);

#endif