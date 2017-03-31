#include "mechanism.h"
#include <stdio.h>

/* On verifie que le coup demande est bien valide. */
int isMoveValid( gameboard_t gameboard, action_t takeSticks )
{
    if(takeSticks >= gameboard.nSticks || takeSticks <= 0 || takeSticks > MAX_STICKS)
        return 0;
    else
        return 1;
}
