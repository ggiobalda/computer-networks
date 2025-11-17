# Trascrizione note a voce sulle specifiche di progetto
## Generale
Si assume che la lavagna è la prima cosa ad essere eseguita, quindi il caso in cui lavagna non è attiva e viene avviato un utente è da considerarsi come funzione aggiuntiva non richiesta.

## Specifiche per matricole pari
La lavagna, appena creata, non ha card. Il primo utente che si registra creerà la prima card, poi lui o altri utenti che si connettono creeranno altre cards fino ad arrivare almeno a 10. Quando il numero di utenti è >= 1 (quindi eccetto il primo, che si connette a lavagna vuota), questi devono scambiare messaggi col server per conoscere le card presenti.

### Workflow
C'è la lavagna, si collegano gli utenti, quando c'è più di un utente la lavagna comunica a tutti gli user la lista delle card in todo, gli utenti a sto punto si scambiano info per decidere chi fa cosa. Raggiunto il consenso mandano il messaggio alla lavagna, dove ogni utente dice che card si prende in carico.

### Comandi
- **available_card**: Serve a triggerare l'assegnamento attraverso consenso dell'attività tra tutti gli utenti connessi. appena viene registrata una nuova card, se presente più di 1 utente, si notifica a tutti gli utenti questa card.
- **choose_user**: utente associa il suo costo alla card correntemente assegnata, ogni card sarà assegnata all'utente che ha costo minore. in caso di parità si sceglie quello con porta minore
- **ack_card**: ogni utente ha la lista dei costi per ogni card, e se è lui quello con costo minore per una certa card allora manda ack al server per comunicare che è lui a prenderla in carico
