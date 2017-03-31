

/*  Doubly-linked lists/fifos/lifos of anything in ANSI C preprocessor macros       */
/*  Copyright (C) 2007  Francois Cayre                                              */
/*                                                                                  */
/*  This program is free software; you can redistribute it and/or                   */
/*  modify it under the terms of the GNU General Public License                     */
/*  as published by the Free Software Foundation; either version 2                  */
/*  of the License, or (at your option) any later version.                          */
/*                                                                                  */
/*  This program is distributed in the hope that it will be useful,                 */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                  */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                   */
/*  GNU General Public License for more details.                                    */
/*                                                                                  */
/*  You should have received a copy of the GNU General Public License               */
/*  along with this program; if not, write to the Free Software                     */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */


#ifndef _TRICKLIST_H_
#define _TRICKLIST_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define OUTPUT                                        stderr

#define NONINIT    "Error: List/FIFO/LIFO not initialized\n"
#define NODATA        "Warning: No data in list/FIFO/LIFO\n"


/* Remember those ol' good IBM days? */
#define POISON_P                        ((void *)0xdeadbeef)
#define POISON_N                        ((void *)0xbeefdead)

/* Without loss of generality */
typedef struct cell_t {
  struct cell_t * prev, * next; 
  void * content; 
} *list;


/* Convenience types */
typedef list fifo;
typedef list lifo; 


/* Like this one? */
#define _idx_value(target,object,idx)                                                      \
           *((int*)(((char*)target)+((unsigned int)((char*)&object.idx-(char*)&object))))

#define _new_cell(l) do {                                                                  \
          (l)=malloc(sizeof(*(l)));                                                        \
          (l)->prev=POISON_P;                                                              \
          (l)->next=POISON_N;                                                              \
          (l)->content=NULL;   /* empty cell */                                            \
        } while(0)


#define list_init(l) _new_cell((l))

/* Just make sure the list exists and contains something before proceeding */
#define foreach(l,i) if ( (l) && (l)->content!=NULL )                                      \
                       for ( (i)=(l) ; (i)!=POISON_N ; (i)=(i)->next )

/* Apply any function on list/fifo/lifo cells */
#define list_apply_function(l,function) do {                                               \
          list i;                                                                          \
	  if(!(l)) fprintf(OUTPUT, NONINIT);                                               \
          else {                                                                           \
	    if ( (l)->prev==POISON_P && (l)->next==POISON_N && (l)->content==NULL)         \
	      fprintf(OUTPUT, NODATA);                                                     \
	    else                                                                           \
	      foreach(l,i) {						                   \
	        function((i)->content);                                                    \
            }                                                                              \
          }                                                                                \
        } while(0)

#define list_add_first(l,object) do {                                                      \
          if ( (l)->content==NULL ) {                                                      \
            (l)->content=malloc(sizeof(object));                                           \
            memcpy((l)->content, &object, sizeof(object));                                 \
          }                                                                                \
          else {                                                                           \
            list new;                                                                      \
            _new_cell(new);                                                                \
            new->content=malloc(sizeof(object));                                           \
            memcpy(new->content, &object, sizeof(object));                                 \
            new->next=(l); (l)->prev=new; (l)=new;                                         \
          }                                                                                \
        } while(0)

#define list_add_last(l,object) do {                                                       \
          if ( (l)->content==NULL ) {                                                      \
            (l)->content=malloc(sizeof(object));                                           \
            memcpy((l)->content, &object, sizeof(object));                                 \
          }                                                                                \
          else {                                                                           \
            list new, ll;                                                                  \
            _new_cell(new);                                                                \
            new->content=malloc(sizeof(object));                                           \
            memcpy(new->content, &object, sizeof(object));                                 \
            ll=l; while (ll->next!=POISON_N) ll=ll->next;                                  \
            ll->next=new;                                                                  \
            new->prev=ll;                                                                  \
          }                                                                                \
        } while(0)

/* 'index' is the name of an integer symbol in a structure being put inside the list */
#define list_add_by_index(l,object,index) do {                                             \
          if ( (l)!=NULL && (l)->content==NULL ) {                                         \
            (l)->content=malloc(sizeof(object));                                           \
            memcpy((l)->content, &object, sizeof(object));                                 \
          }                                                                                \
          else {                                                                           \
            list i=l;                                                                      \
	    while(object.index>_idx_value(i->content,object,index) && i->next!=POISON_N) { \
              i=i->next;                                                                   \
            }                                                                              \
            if ( i->prev==POISON_P && i->next==POISON_N ) {                                \
              if ( object.index > _idx_value(i->content,object,index) )                    \
                list_add_last(l,object); else list_add_first(l,object);                    \
              break;                                                                       \
            }                                                                              \
            else {                                                                         \
              list new;                                                                    \
              if ( i->prev==POISON_P ) {                                                   \
                if ( object.index > _idx_value(i->content,object,index)  ) {               \
                  i=i->next; goto link;                                                    \
                }                                                                          \
                else {                                                                     \
                  list_add_first(l,object); break;                                         \
                }                                                                          \
              }                                                                            \
              if ( i->next==POISON_N ) {                                                   \
                if ( object.index < _idx_value(i->content,object,index)  ) {               \
                  i=i->prev; goto link;                                                    \
                }                                                                          \
                else { list_add_last(l,object); break; }                                   \
              }                                                                            \
              if ( i->prev!=POISON_P && i->next!=POISON_N ) i=i->prev;                     \
              link:                                                                        \
                _new_cell(new);                                                            \
                new->content=malloc(sizeof(object));                                       \
                memcpy(new->content, &object, sizeof(object));                             \
                new->prev=i; new->next=i->next; i->next->prev=new; i->next=new;            \
            }                                                                              \
          }                                                                                \
        } while(0)

#define list_del_first_by_index(l,object,index) do {                                       \
          list i;                                                                          \
          foreach((l),i) {                                                                 \
            if ( object.index==_idx_value(i->content,object,index) ) {                     \
              if ( i->next != POISON_N && i->prev != POISON_P ) {                          \
                i->prev->next=i->next;                                                     \
                i->next->prev=i->prev;                                                     \
                goto liberam;                                                              \
              }                                                                            \
              if ( i->next == POISON_N ) {                                                 \
                i->prev->next=POISON_N;                                                    \
                goto liberam;                                                              \
              }                                                                            \
              if ( i->prev == POISON_P ) {                                                 \
                i->next->prev=POISON_P;                                                    \
                (l) = i->next;                                                             \
                goto liberam;                                                              \
              }                                                                            \
            liberam:                                                                       \
              free(i->content); i->content=NULL; free(i); i=NULL; break;                   \
            }                                                                              \
          }                                                                                \
        } while(0)

#define list_del_all_by_index(l,object,index) do {                                         \
          list i;                                                                          \
          foreach((l),i) {                                                                 \
            if ( object.index==_idx_value(i->content,object,index) ) {                     \
              if ( i->next != POISON_N && i->prev != POISON_P ) {                          \
                i->prev->next=i->next;                                                     \
                i->next->prev=i->prev;                                                     \
                goto liberame;                                                             \
              }                                                                            \
              if ( i->next == POISON_N ) {                                                 \
                i->prev->next=POISON_N;                                                    \
                goto liberame;                                                             \
              }                                                                            \
              if ( i->prev == POISON_P ) {                                                 \
                i->next->prev=POISON_P;                                                    \
                (l) = i->next;                                                             \
                goto liberame;                                                             \
              }                                                                            \
            liberame:                                                                      \
              free(i->content); i->content=NULL; free(i); /* Don't rely on this hack */    \
            }                                                                              \
          }                                                                                \
        } while(0)

#define list_del_first(l) do {                                                             \
          if ( (l)->content!=NULL ) {                                                      \
            if ( (l)->next==POISON_N ) {                                                   \
              free( (l)->content ); (l)->content=NULL;                                     \
            }                                                                              \
            else {                                                                         \
              list ll=l->next;                                                             \
              ll->prev=POISON_P;                                                           \
              free( (l)->content );                                                        \
              free((l));                                                                   \
              (l)=ll;                                                                      \
            }                                                                              \
          }                                                                                \
        } while(0)

#define list_del_last(l) do {                                                              \
          if ( (l)->content!=NULL ) {                                                      \
            if ( (l)->next==POISON_N ) {                                                   \
              free( (l)->content ); (l)->content=NULL;                                     \
            }                                                                              \
            else {                                                                         \
              list s=l, ll=l;                                                              \
              while( ll->next!=POISON_N ) {                                                \
                s=ll; ll=ll->next;                                                         \
              }                                                                            \
             free(ll->content); free(ll);                                                  \
             s->next=POISON_N;                                                             \
            }                                                                              \
          }                                                                                \
        } while(0)
 
#define list_print(l) do {                                                                 \
          if ( !(l) ) {                                                                    \
            fprintf(OUTPUT, NONINIT); break;                                               \
          }                                                                                \
  	  if ( (l)->prev==POISON_P && (l)->next==POISON_N && (l)->content==NULL )          \
	    fprintf(OUTPUT, NODATA);                                                       \
	  else {                                                                           \
            list i;                                                                        \
            foreach(l,i) {                                                                 \
              fprintf(OUTPUT, "Cell @ %p (prev @ %p, next @ %p): content @ %p\n",          \
              (void *)i, (void *)(i->prev), (void *)(i->next), (void *)(i->content));      \
            }                                                                              \
	  }                                                                                \
        } while(0)

#define list_delete(l) do {                                                                \
          if ( (l) ) {                                                                     \
            while( (l)->content!=NULL ) list_del_first(l);                                 \
            free((l)); (l)=NULL;                                                           \
          }                                                                                \
        } while(0)

#define list_check_if_empty(l,test) do {                                                   \
          if ( (l)->content!=NULL ) (test)=0; else (test)=1;                               \
        } while(0)

#define list_push(l,object) list_add_first(l,object)

#define list_pop(l,object) do {                                                            \
          if ( (l) && (l)->content!=NULL ) {                                               \
            memcpy(&object, (l)->content, sizeof(object));                                 \
            list_del_first((l)); }                                                         \
        } while(0)

#define fifo_pop(l,object) list_pop(l,object)

#define lifo_push(l,object) list_add_last(l,object)

#define lifo_pop(l,object) list_pop(l,object) 

#define fifo_init(l) list_init(l)

#define lifo_init(l) list_init(l)

#define fifo_delete(l) list_delete(l)

#define lifo_delete(l) list_delete(l) 

#define fifo_apply_function(l,function) list_apply_function(l,function)

#define lifo_apply_function(l,function) list_apply_function(l,function)

#define fifo_print(l) list_print(l)

#define lifo_print(l) list_print(l)

#endif /* _TRICKLIST_H_ */
