#include "../../../include/handlers/client/p2p_auction_handlers.h"

// variabili di stato utili per l'asta
static int am_i_busy = 0;
static int my_port_global = 0;
static int auction_card_id = -1;
static int bids_collected = 0;
static int bids_expected = 0;
static int current_min_cost = -1;
static int current_winner_port = -1;

void p2p_msg_handler(int p2p_listener_sock, int server_socket) {
    // nuovo socket per peer
    int peer_sock;
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(peer_addr);

    // 1. Accetto la connessione del peer
    if ((peer_sock = accept(p2p_listener_sock, (struct sockaddr *)&peer_addr, &addr_len)) < 0) {
        perror("Errore accept socket P2P\n");
        return;
    }

    // ricezione messaggio
    MsgHeader head;
    char buffer[MAX_PAYLOAD_SIZE];
    if (recv_msg(peer_sock, &head, buffer, sizeof(buffer)) <= 0) {
        perror("Errore ricezione messaggio da peer\n");
        return;
    }

    // controllo header
    if (head.type != UtU_COST) {
        perror("Ricevuto messaggio sconosciuto da peer\n");
        return;
    }

    MsgChooseUserPayload* bid = (MsgChooseUserPayload*)buffer;      
    
    // controllo di sicurezza che l'asta sia per la carta corrente
    if (bid->card_id != auction_card_id) {
        perror("Ricevuto messaggio da peer per un'altra asta\n");
        return;
    }
    
    printf("[ASTA] Ricevuto costo %d da porta %d\n", bid->cost, bid->sender_port);
    
    // aggiornamento costo minimo e porta associata
    if (bid->cost < current_min_cost) {
        current_min_cost = bid->cost;
        current_winner_port = bid->sender_port;
    }
    else if (bid->cost == current_min_cost) {
        if (bid->sender_port < current_winner_port) {
            current_winner_port = bid->sender_port;
        }
    }
    
    // incremento contatore e controllo fine asta
    bids_collected++;
    check_auction_winner(server_socket);
    
    close(peer_sock);
}

void check_auction_winner(int server_socket) {
    if (bids_collected >= bids_expected) {
        printf("Asta conclusa per card %d\n", auction_card_id);

        if (current_winner_port == my_port_global) {
            am_i_busy = 1;
            printf("HO VINTO IO! Invio ACK alla lavagna\n");
            
            // invio ACK
            MsgAckCardPayload ack;
            ack.card_id = auction_card_id;
            send_msg(server_socket, UtB_ACK_CARD, &ack, sizeof(ack));
            
            // simulazione lavoro con sleep
            printf("Inizio esecuzione task (Sleep 5s)...\n");
            sleep(5); 
            
            // invio DONE
            send_msg(server_socket, UtB_CARD_DONE, NULL, 0);
            printf("Lavoro terminato. Inviato CARD_DONE\n");

            am_i_busy = 0;
        }
        else {
            printf("Asta persa. Porta vincitore: %d, costo: %d\n", current_winner_port, current_min_cost);
        }
        
        auction_card_id = -1;
    }
}

void response_available_card_handler(MsgHeader* head, char* payload, int server_socket, int my_port) {
    MsgAvailableCardPayload* msg = (MsgAvailableCardPayload*)payload;
    
    printf("\n[ASTA] Nuova card disponibile: %d - \"%s\"\n", msg->card_id, msg->description);
    
    // calcolo costo: se già occupato imposto al massimo per perdere asta
    int my_cost;
    srand(time(NULL));
    if (am_i_busy) {
        my_cost = COST_MAX;
        printf("[ASTA] Sono occupato. Costo impostato a INT_MAX.\n");
    }
    else {
        my_cost = rand() % COST_MAX;
        printf("[ASTA] Sono libero. Il mio costo random è: %d\n", my_cost);
    }
    
    // set variabili per l'asta
    am_i_busy = 0;
    my_port_global = my_port;
    auction_card_id = msg->card_id;
    bids_collected = 0;
    bids_expected = msg->n_users-1;
    current_min_cost = my_cost;
    current_winner_port = my_port;

    // invio costo a tutti i peer
    MsgChooseUserPayload p2p_msg;
    p2p_msg.card_id = msg->card_id;
    p2p_msg.cost = my_cost;
    p2p_msg.sender_port = my_port;

    for (int i = 0; i < msg->n_users; i++) {
        // salto me stesso
        if (msg->users_ports[i] == my_port)
            continue;

        // setup socket p2p
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            continue;
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(msg->users_ports[i]);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        // connessione: in caso di errore scarto il peer
        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("[ASTA] Errore connessione a peer");
            bids_expected--;
            close(sock);
            continue;
        }
        
        // invio costo
        send_msg(sock, UtU_COST, &p2p_msg, sizeof(p2p_msg));
        
        printf("[ASTA] Inviato costo %d a porta %d\n", my_cost, msg->users_ports[i]);
        close(sock);
    }

    // controllo di sicurezza nel caso in cui nessun peer mi invia costo e quindi check_win non verrebbe mai chiamata
    check_auction_winner(server_socket);
}