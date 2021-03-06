#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include "message.h"
#include "mechanism.h"


void * sharedMemoryAddr = NULL;


int sigalarm_handler(int sigNum)
{
    printf("No connection to server.\nExiting game.\n");
    exit(EXIT_FAILURE);
} 


void detachSharedMemory()
{
    /* Pour que le serveur de jeu puisse detruire le segment de memoire 
       partagee, tous les processus l'ayant attache doivent le detacher. */
    
    /* TODO : Detacher le segment de memoire partagee avec shmdt() */
	shmdt(sharedMemoryAddr);
}


int main(int argc, char * argv[])
{
    key_t key;
    int queueId;
    int sharedMemoryId;
#ifdef _SMART
    int Stck=30;
#endif
    msg_t message;

    action_t takeSticks;
    int winnerPid = 0;

    if(signal(SIGALRM, (void *) sigalarm_handler) == SIG_ERR)
    {
        fprintf(stderr, "Failed to associate SIGALRM to handler.\n");
    }
    
    /* 
       Rejoindre la file de message
       ----------------------------
        1 - Creer la meme clef que pour le serveur
        2 - Rejoindre la file de message correspondant à la clef
    */

    /* TODO : ftok() */ 
	key=ftok("./file_test.f",1);

    /* TODO : msgget()
       Le serveur ayant deja cree la file, pas besoin de flags ce coup-ci. 
    */
	queueId = msgget(key,0);
    /* On dit bonjour au serveur en se presentant. */ 
    message.type = 1;
    sprintf(message.data, "%d", getpid());

    /* TODO : msgsnd -- envoyer le message */
	msgsnd(queueId,&message,sizeof(message)-sizeof(long),0);

    /* On attend sa reponse. S'il n'y en a pas, c'est qu'il ne doit pas y
       avoir de serveur à l'autre bout de la file de message. Pour eviter
       d'attendre indefiniment une reponse, on enclenche une alarme. */
    alarm(3);
    /* TODO : msgrcv -- recevoir un message (type = getpid()) */
	msgrcv(queueId,&message, sizeof(message)-sizeof(long) ,getpid(),0);


    /* Message recu, on desactive l'alarme. */
    alarm(0); 

    /* On récupère l'identifiant necessaire pour attacher le segment de
       memoire partagee contenant le plateau de jeu. */
    sscanf(message.data, "%d", &sharedMemoryId);

    /* TODO : attacher le segment de memoire partagee à l'aide de shmat() */
	 
   	
	sharedMemoryAddr= shmat(sharedMemoryId,NULL,0);
   gameboard_t * gameboard = (gameboard_t *) sharedMemoryAddr;
   	

	atexit(detachSharedMemory);

    printf("Connection to the game server: success.\n");

    srand(getpid());
    while(!winnerPid)
    {
        /* TODO : msgrcv -- recevoir un message (type =  getpid()) */


	msgrcv(queueId,&message, sizeof(message)-sizeof(long) ,getpid(),0);
        if(!strncmp("END", message.data, 3))
        {
            sscanf(message.data, "END %d", &winnerPid);
            if(winnerPid == getpid())
                printf("You win!\n");
            else
                printf("Looser!\n");
            break;
        }

        printf("%d sticks remaining. ", gameboard->nSticks);
        
        /* C'est ici qu'on dote nos joueurs d'une technique plus ou moins
          efficace. */

#ifdef _ODD
        /* Il faut toujours un nombre impair de batonnets ! Toujours ! */
        do{scanf("%d", &takeSticks);}while(takeSticks>3 || takeSticks < 1);
#endif

#ifdef _RANDOM
        /* La hasard fait bien les choses. Parfois. */
        takeSticks = (rand() % MAX_STICKS) + 1;
#endif

#ifdef _SMART
        /* TODO : coder une IA avec un peu de jugeote. */
		if(gameboard->nSticks == N_STICKS ){
			Stck=N_STICKS;
		}
		if (Stck-gameboard->nSticks == 0){
			takeSticks = 1;
		}
		else{
			takeSticks = 4-(Stck-gameboard->nSticks);
		}
		Stck=gameboard->nSticks - takeSticks;
#endif

        message.type = 1;
        sprintf(message.data, "%d", takeSticks);
        printf("Taking %d of them.\n", takeSticks);

        /* TODO : msgsnd -- envoyer l'action à jouer */
	
	msgsnd(queueId,&message,sizeof(message)-sizeof(long),0);
    }

    return EXIT_SUCCESS;
}
