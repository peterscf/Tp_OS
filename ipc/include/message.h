#ifndef _MESSAGE_H
#define _MESSAGE_H

/* Des constantes qui pourraient servir à ftok(). */
#define KEY_FILE "./server.c"
#define KEY_QUEUE 1008
#define KEY_SHARED_MEMORY 1009

#define MSG_LENGTH 32 

/* La structure des messages que s'enverront nos processus. */
typedef struct MSG {
    long type;
    char data[MSG_LENGTH];
} msg_t;

#endif
