#ifndef _MECHANISM_H_
#define _MECHANISM_H_

#define N_PLAYERS   2 
#define N_STICKS   30 
#define MAX_STICKS  3

/* La structure du plateau de jeu. */
typedef struct GAMEBOARD {
    int nSticks;
} gameboard_t;

typedef int action_t;

int isMoveValid( gameboard_t gameboard, action_t takeSticks );

#endif
