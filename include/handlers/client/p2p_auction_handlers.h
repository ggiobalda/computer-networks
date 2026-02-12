/**
 * @file p2p_auction_handlers.h
 * @author Giorgio Baldacci
 * @brief Funzioni handler chiamate dall'utente per gestire socket peer
 * 
 */
#ifndef P2P_AUCTION_HANDLERS_H
#define P2P_AUCTION_HANDLERS_H

#include "../../network/utils.h"

/**
 * @brief Handler per gestire messaggi P2P in ingresso
 * 
 * Registra costo e porta se offerta attualmente migliore
 * 
 * @param p2p_listener_sock Descrittore socket p2p dell'utente
 * @param server_socket Descrittore socket del server
 * 
 */
void p2p_msg_handler(int p2p_listener_sock, int server_socket);

/**
 * @brief Controlla se chiamante ha vinto l'asta
 * 
 * In tal caso manda ACK al server, simula lavoro e poi manda CARD DONE al server
 * 
 * @param server_socket Descrittore socket del server
 * 
 */
void check_auction_winner(int server_socket);

/**
 * @brief Gestisce messaggio AVAILABLE CARD inviato dalla lavagna
 * 
 * Questo avvia l'asta, quindi calcola costo e lo invia a tutti gli utenti in lista
 * 
 * @param head Puntatore all'header della risposta
 * @param payload Puntatore al payload della risposta
 * @param server_socket Descrittore socket del server
 * @param my_port Porta dell'utente chiamante
 * 
 */
void response_available_card_handler(MsgHeader* head, char* payload, int server_socket, int my_port);

/**
 * @brief Calcola il timeout per la select nel main
 * @param tv Puntatore alla struct timeval da riempire
 * 
 * @return Puntatore a tv se c'è un lavoro in corso, NULL se l'utente è libero
 * 
 */
struct timeval* get_task_timer(struct timeval* tv);

/**
 * @brief Controlla se il lavoro simulato è finito
 * @param server_socket Socket per inviare CARD_DONE
 * 
 */
void check_task_completion(int server_socket);

#endif