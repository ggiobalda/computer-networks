#include "../../include/network/utils.h"

const char* ColumnNames[] = {"TO DO", "DOING", "DONE"};

/**
 * @brief Funzione helper interna per leggere esattamente n bytes dal socket
 * 
 * @return Numero bytes letti, error value altrimenti
 */
int read_n_bytes(int socket, void* buffer, int n) {
    int bytes_read = 0;
    char* buf = (char*)buffer;
    
    // ciclo di letture
    while (bytes_read < n) {
        int res = read(socket, buf + bytes_read, n - bytes_read);
        if (res < 1)
            return res;
        bytes_read += res;
    }
    
    return bytes_read;
}

/**
 * @brief Funzione helper interna per scrivere esattamente n bytes nel socket
 * 
 * @return Numero bytes scritti, error value altrimenti 
 */
int write_n_bytes(int socket, const void* buffer, int n) {
    int bytes_written = 0;
    char* buf = (char*)buffer;
    
    // ciclo di scritture
    while (bytes_written < n) {
        int res = write(socket, buf + bytes_written, n - bytes_written);
        if (res < 1)
            return res;
        bytes_written += res;
    }
    
    return bytes_written;
}

int send_msg(int socket, MsgType type, const void* payload, int payload_len) {
    // controllo input
    if (payload_len > 0 && payload == NULL) {
        perror("Argomenti non validi\n");
        return -1;
    }

    // inizializza e invia header
    MsgHeader head;
    head.type = type;
    head.payload_len = payload_len;
    int res;
    if ((res = write_n_bytes(socket, &head, sizeof(MsgHeader))) < 1)
        return res;

    // invio payload
    if (payload_len > 0 && ((res = write_n_bytes(socket, payload, payload_len)) < 1))
        return res;
    
    return 1; 
}

int recv_msg(int socket, MsgHeader* head, void* payload, int MAX_PAYLOAD_SIZE_len) {
    // controllo input
    if (head == NULL || payload == NULL) {
        perror("Argomenti non validi\n");
        return -1;
    }
    
    // ricezione header
    int ret;
    if ((ret = read_n_bytes(socket, head, sizeof(MsgHeader))) < 1)
        return ret;

    // ricezione payload (se presente)
    if (head->payload_len > 0) {   
        if (MAX_PAYLOAD_SIZE_len < head->payload_len) {
            perror("Dimensione buffer insufficiente\n");
            return -1;
        }
        if ((ret = read_n_bytes(socket, payload, head->payload_len)) < 1)
            return ret;
    }
        
    return 1;
}
