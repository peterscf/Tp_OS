#ifndef _UTILS_H
#define _UTILS_H__

/* Verifie si le demon n'est pas deja en train de tourner */
int check_if_launched( void );

/* cree le fichier contenant le PID du demon, cf. ci-dessus */
void create_lock_file( void );


/* Les fonctions suivantes sont appelees lors de la sortie
   du processus, lors de l'appel a la fonction exit()
 */

/* efface le fichier contenant le PID du demon */
void delete_lock_file( void );

/* detruit la liste des evenements */
void delete_event_list( void );

/* affiche qu'on termine le demon */
void finish( void );

#endif
