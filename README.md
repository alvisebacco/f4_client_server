# f4_client_server
Four in a row game, with system call, 2 players 
# F4

Il programma F4 e' una simulazione del gioco "Forza4" scritto per lo studio delle System call Linux.

Il progetto si articola di due programmi principali, il client e il server.
Il server fa da arbitro ed istanzia i segmenti di memoria condivisa ed i propri semafori, il client si deve istanziare almeno due volte (una per giocatore) per iniziare la partita.

## Contenuto della cartella di progetto

![contenuto](contenuto.png)

Per buildare l'applicazione si puo' utilizzare
```bash
./make.sh
```
## Il primo avvio del server

La prima operazione da dover fare per iniziare a giocare e' quella di istanziare il server.
Esempio di avvio:
```bash
./server 15 20 69 XO
```
15 e 20 sono le dimensioni dell'area di gioco espressi in righe e colonne (non possono essere inferiori a 5x5).
69 e' l'identificatore del segmento di memoria che si intende istanziare.
X e O sono i simboli di gioco che utilizzeranno Player1 e Player2.

Dopo l'avvio, il server rimane in attesa che il primo e il secondo giocatore joinino la partita, esponendo le informazioni per il collegamento.

![contenuto](serverr.png)

## Il primo avvio dei client

I client, di conseguenza, verranno avviati in questo modo:
```bash
./client mario 69 20
./client luigi 69 21
```
![first](first.png)

Dopo che il secondo giocatore si sara' unito alla partita, server e clients si scambiano le informazioni per iniziare il gioco. Il server ricava il PID dei client e si fa cominicare i nomi dei giocatori. I client ottengono i loro simboli di gioco e lo schema della matrice.

![info](info.png)

## La prima mossa

Dopo lo scambio di informazioni il Player1 inizializzera' la matrice di gioco in base ai dati ricevuti, e rimarra' in attesa che il giocatore faccia la prima mossa.

![first](firstmove.png)

Il giocatore puo' inserire il valore 100 per lasciare che il programma faccia una mossa casuale.

## Lo scambio di mosse e la valutazione della vittoria

Ad ogni mossa la matrice viene scritta dai client nella memoria condivisa, prelevata dal server che valuta la vittoria. Il server mostra tutte le informazioni di debug, permettendo di capire come viene valutata la vittoria:

![win](win.png)
![win1](win1.png)
![win2](win2.png)

Il server valuta le vittorie in obliquo, verticale ed orizzontale, ruotando la matrice di 90 e 135 gradi.

## La vittoria

![super](super.png)

Il server rilevando una vittoria comunica ai relativi client chi e' il vincitore, concludendo eliminando la memoria condivisa e i semafori.

## Ctrl-c

Alla doppia pressione del Ctrl-c sul server il processo termina pulendo le risorse condivise.

![ctr](ctrl.png)
