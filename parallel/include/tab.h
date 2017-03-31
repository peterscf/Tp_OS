#ifndef _TAB_H_
#define _TAB_H_

/* Fonctions destinees a manipuler des tableaux.
   Servent pour l'algorithme du boulanger. */

/* Affiche le contenu du tableau passe en parametre.
   Le second parametre est la taille du tableau. */
void print_tab( unsigned int * tab, unsigned int sz ); 

/* Renvoie le maximum des valeurs contenus dans le tableau. 
   Le second parametre est la taille du tableau. */
unsigned int max( unsigned int * tab, unsigned int sz ); 

#endif
