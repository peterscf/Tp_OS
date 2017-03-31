#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#include "mechanism.h"
#include "message.h"

int          queueId        = -1;
int          sharedMemoryId = -1;
gameboard_t *gameboard      = NULL;

/* On ne laisse rien trainer en partant. En particulier, on pensera a
   detacher et detruire le segment de memoire partagee et à detruire la 
   file de message.
*/  
void closeQueue()
{
    /* TODO : Detruire la file de message avec msgctl() */
}


void closeSharedMemory()
{
    /* TODO : Detruire le segment de memoire partagee avec shmctl() */
}


void detachSharedMemory()
{
    /* Pour que le serveur de jeu puisse detruire le segment de memoire 
       partagee, tous les processus l'ayant attache doivent le detacher. */
    
    /* TODO : Detacher le segment de memoire partagee avec shmdt() */
}


int main(int argc, char *argv[])
{
    key_t keyQueue;
    key_t keySharedMemory;

    msg_t message;

    int player = 0;
    int winner = 0;
    int pid[N_PLAYERS];

    action_t takeSticks;

    /* 
       Creation d'une file de messages
       -------------------------------
        1 - Creer une clef permettant a l'OS d'identifier la file de 
            messages -- ftok()
        2 - Ouvrir la file de message correspondant a la clef -- msgget()
    */

    /* TODO : ftok() */ 


    /* TODO : msgget() 
       On veut creer la file avec des droits en lecture et écriture. On 
       utilisera donc les flags IPC_CREAT | 0666. 
    */


    atexit(closeQueue);

    /* 
       Creation d'un segment de memoire partagee
       -----------------------------------------
        1 - Creer une clef permettant a l'OS d'identifier le segment -- ftok()
        2 - Creer le segment correspondant a la clef -- shmget()
        3 - Attacher le segment au processus et récupérer son adresse -- shmat()
    */

    /* TODO : ftok() */ 


    /* TODO : shmget() 
       On veut creer le segment avec des droits en lecture et écriture pour le
       serveur et en lecture pour les autres. On utilisera donc les flags 
       IPC_CREAT | 0622. De plus, le segment de memoire partagee contiendra
       uniquement le plateau de jeu du type gameboard_t. 
    */

    
    /* TODO : shmat() */ 


    atexit(closeSharedMemory);
    atexit(detachSharedMemory);


    /* Attente de connexion des joueurs */
    printf("Waiting for %d players.\n", N_PLAYERS);

    while(player < N_PLAYERS)
    {
        /* TODO : msgrcv -- recevoir un message (type = 1) */


        sscanf(message.data, "%d", &pid[player]);
        printf("Connection from player %d, with pid %d.\n", player, pid[player]);

        message.type = pid[player];
        sprintf(message.data, "%d", sharedMemoryId);
        
        /* TODO : msgsnd -- envoyer le message contenant l'identifiant
           permettant d'attacher le segment de memoire partagee. */


        player++;
    }

    /* Quand tout le monde est là, la partie commence */
    printf("Let the game start with %d sticks.\n", N_STICKS);

    gameboard->nSticks = N_STICKS;
    player = 0;

    while(gameboard->nSticks > 1)
    {
        message.type = pid[player];
        sprintf(message.data, "PLAY");
        /* TODO : msgsnd -- envoyer le message */



        /* TODO : msgrcv -- recevoir le message contenant le coup a jouer */


        sscanf(message.data, "%d", &takeSticks);
        if(isMoveValid(*gameboard, takeSticks))
        {
            gameboard->nSticks = gameboard->nSticks - takeSticks;
            printf("Player %d (pid %d) takes %d sticks. %d remaining.\n", player, pid[player], takeSticks, gameboard->nSticks);

            player = (player + 1) % N_PLAYERS;
        }

        /* Pas si vite, sinon on ne voit pas ce qu'il se passe. */
        sleep(1);
    }

    /* Le gagnant, c'est celui qui n'a pas pris le dernier batonnet. */
    player = (player + N_PLAYERS - 1) % N_PLAYERS;
    winner = pid[player];
    printf("Player %d (pid %d) wins!\n", player, winner);


    /* On previent les joueurs que la partie est terminee et on donne
       le PID du vainqueur. */
    player = 0;

    while(player < N_PLAYERS)
    {
        message.type = pid[player];
        sprintf(message.data, "END %d", winner);

        /* TODO : msgsnd -- envoyer le message */


        player++;
    }

    return EXIT_SUCCESS;
}
