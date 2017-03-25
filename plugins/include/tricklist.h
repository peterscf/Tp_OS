/*
  TrickList: ANSI C macros for handling lists, lifos, fifos and hash tables. 
  Copyright (C) 2007  Francois Cayre

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
  Boston, MA  02110-1301, USA.

  Contact: francois.cayre@gipsa-lab.inpg.fr

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef _TRICKLIST_H_
#define _TRICKLIST_H_ 

#ifdef __cplusplus
extern "C" {
#endif

#define __TRICKLIST_MAXLEN 12

#define __TRICKLIST_OUTPUT stderr

typedef struct cell_t 
{
  struct cell_t * prev, * next; 
  void * content; 
  unsigned short csz; 
} *cell, *list; 

typedef struct list_header_t 
{
  char * lname; 
  FILE * out; 
  unsigned int count; 
} list_header; 

typedef struct head_t 
{
  list_header h; 
  struct cell_t data; 
} * head; 

typedef list fifo; 
typedef list lifo; 
typedef list set; 


#define __object_set_field( o, field, value ) do {			\
    (o).field = value;							\
  } while ( 0 ) 


#define __object_get_field( o, field, value ) do {	\
    value = (o).field;					\
  } while ( 0 )

#define __get_header(l) ((head)( (char *)(l)-sizeof( list_header )))

#define __idx_value(target,object,idx)					\
  (&(target)?*((int*)(((char*)(target))+((unsigned int)((char*)&(object).idx-(char*)&(object))))):-1)

#define __new_cell( c )	do {			\
    (c) = malloc( sizeof( *(c) ) );		\
    (c)->content = NULL;			\
    (c)->prev = (c);				\
    (c)->next = (c);				\
  } while ( 0 ) 

#define __cell_print( c, out ) do {						\
    fprintf( (out), "cell @ %p: prev @ %p, next @ %p (content @ %p, %d bytes)\n", (void*)(c), (void*)(c)->prev, (void*)(c)->next, (void*)(c)->content, (c)->csz ); \
  } while ( 0 )

#define __list_init_prealloc( l ) do {					\
    head hl = malloc( sizeof( *hl ) );					\
    hl->h.count = 0;							\
    hl->h.lname = NULL;							\
    hl->h.out = __TRICKLIST_OUTPUT;					\
    (l) = (list)&(hl->data);						\
    (l)->content = NULL;						\
    (l)->prev = l;							\
    (l)->next = l;							\
  } while ( 0 )

#define __list_init( l ) __list_init_prealloc( l ) 

#define __list_get_count( l, cnt ) do {				\
    head hl = __get_header(l);					\
    assert( sizeof( (cnt) ) == sizeof( hl->h.count ) );		\
    memcpy( &(cnt), &(hl->h.count), sizeof( (cnt) ) );		\
  } while ( 0 ) 

#define __list_decr_count( l ) do {			\
    head hl = __get_header(l);				\
    hl->h.count--;					\
  } while ( 0 )						\

#define __list_set_count( l, n ) do {		\
      head hl = __get_header(l);		\
      hl->h.count = n;				\
  } while ( 0 ) 

#define __list_incr_count( l ) do {			\
    head hl = __get_header(l);				\
    hl->h.count++;					\
  } while ( 0 )						\

#define __list_set_output( l, file ) do {		\
    head hl = __get_header(l);				\
    fprintf( stderr, "Warning: Changing output!\n" );	\
    hl->h.out = (file);					\
  } while( 0 )

#define __list_set_name( l, newname ) do {				\
    unsigned int i, len;						\
    head hl = __get_header(l);						\
    len = strlen(newname)+1<__TRICKLIST_MAXLEN?strlen(newname):__TRICKLIST_MAXLEN; \
    if ( hl->h.lname )							\
      {									\
	fprintf( stderr, "Warning: Changing name!\n" );			\
	free( hl->h.lname );						\
      }									\
    hl->h.lname = malloc( (1+strlen(newname))*sizeof(char) );		\
    for ( i= 0; i< len; i++ ) hl->h.lname[i] = newname[i];		\
    hl->h.lname[i] = '\0';						\
  } while ( 0 )								\

#define __list_print_header( l ) do {					\
    head hl = __get_header(l);						\
    if ( hl->h.lname )							\
      fprintf( hl->h.out, "List %s @ %p ", hl->h.lname, (void*)(l) );	\
    else								\
      fprintf( hl->h.out, "List @ %p ", (void*)l );			\
    if ( hl->h.count==1 || hl->h.count==0)				\
      fprintf( hl->h.out, "(%d item)\n", hl->h.count );			\
    else								\
      fprintf( hl->h.out, "(%d items)\n", hl->h.count );		\
  } while ( 0 )								\

#define __list_is_empty( l, empty ) do {	\
    __list_get_count( l, empty );		\
    empty = !empty;				\
  } while ( 0 ) 

#define __list_add_first( l, object ) do {				\
    cell c;								\
    __new_cell( c );							\
    c->next = (l)->next;						\
    c->prev = (l);							\
    (l)->next->prev = c;						\
    (l)->next = c;							\
    c->content = malloc( sizeof( object ) );				\
    c->csz = sizeof(object);						\
    memcpy( (c->content), &(object), sizeof( object) );			\
    __list_incr_count( l );						\
  } while(0)

#define __list_add_last( l, object ) do {			\
    cell c;							\
    __new_cell( c );						\
    c->prev = (l)->prev;					\
    c->next = (l);						\
    (l)->prev->next = c;					\
    (l)->prev = c;						\
    c->content = malloc( sizeof( object ) );			\
    c->csz = sizeof(object);					\
    memcpy( c->content, &(object), sizeof( object) );		\
    __list_incr_count( l );					\
} while(0)

#define __list_add_by_index( l, object, index ) do {			\
    cell c;								\
    cell i = (l)->next;							\
    while( i->content && (object).index > __idx_value( i->content, object, index ) ) \
      i = i->next;							\
    i = i->prev;							\
    __new_cell( c );							\
    c->next = (i)->next;						\
    c->prev = (i);							\
    (i)->next->prev = c;						\
    (i)->next = c;							\
    c->content = malloc( sizeof( object ) );				\
    c->csz = sizeof(object);					\
    memcpy( c->content, &(object), sizeof( object) );			\
    __list_incr_count( l );						\
  } while ( 0 )

#define __list_add_by_index_reverse( l, object, index ) do {		\
    cell c;								\
    cell i = (l)->next;							\
    while( i->content && (object).index < __idx_value( i->content, object, index ) ) \
      i = i->next;							\
    i = i->prev;							\
    __new_cell( c );							\
    c->next = (i)->next;						\
    c->prev = (i);							\
    (i)->next->prev = c;						\
    (i)->next = c;							\
    c->content = malloc( sizeof( object ) );				\
    c->csz = sizeof(object);					\
    memcpy( c->content, &(object), sizeof( object) );			\
    __list_incr_count( l );						\
  } while ( 0 )

#define __list_del_first_by_index( l, object, idx ) do {		\
    cell c;								\
    cell i = (l)->next;							\
    while( i->content && (object).idx != __idx_value( i->content, object, idx ) ) \
      i = i->next;							\
    if ( &(object) )							\
      memcpy( &(object), (i)->next->content, sizeof( (object) ) );	\
    if ( (i)->next->content ) {						\
      free( (i)->next->content );					\
      c = (i)->next;							\
      (i)->next->next->prev = (i);					\
      (i)->next = (i)->next->next;					\
      free( c );							\
      __list_decr_count( l );						\
    }									\
  } while ( 0 ) 

#define list_find_by_value( l, o, idx, match, val, type ) ({	\
      cell i = (l)->next;					\
      int found = 0;						\
      for ( i = (l)->next; (i)->content; i = i->next ) {	\
	if ( 0 == match( val, ((type*)(i->content))->idx ) ) {	\
	  memcpy( &(o), i->content, sizeof(type) );		\
	  found = 1;						\
	  break;						\
	}							\
      }								\
      found; })


#define __list_del_all_by_index( l, object, idx ) do {			\
    cell i = (l)->next, c;						\
    while( i->content ) {						\
      if ( (object).idx==__idx_value(i->content,object,idx) ) {		\
	if ( &(object) )						\
	  memcpy( &(object), i->content, sizeof( (object) ) );		\
	free( i->content );						\
	c = i;								\
	i->next->prev = i->prev;					\
	i->prev->next = i->next;					\
	free( c );							\
	__list_decr_count( l );						\
      }									\
      i = i->next;							\
    }									\
  } while ( 0 )


#define __list_foreach_forward( l, i )				\
  for( (i)=(l)->next; (i)->content; (i) = (i)->next ) 

#define __list_foreach_reverse( l, i )				\
  for( (i)=(l)->prev; (i)->content; (i) = (i)->prev ) 

#define __list_foreach( l, i ) __list_foreach_forward( l, i )

#define __list_print( l ) do {						\
    cell c;								\
    head hl = __get_header(l);						\
    unsigned int sz = sizeof(*hl)+(hl->h.lname?(1+strlen(hl->h.lname)):0); \
    unsigned int csz = 0;						\
    __list_print_header( l );						\
    __list_foreach( l, c ) {						\
      __cell_print( c, hl->h.out );					\
      csz+= c->csz;							\
      sz+= sizeof(*c);							\
      }									\
  } while ( 0 )							

#define _save_list_print( l ) do {					\
    cell c;								\
    head hl = __get_header(l);						\
    unsigned int sz = sizeof(*hl)+((hl->h.lname)?1+strlen(hl->h.lname):0); \
    unsigned int csz = 0;						\
    __list_print_header( l );						\
    __list_foreach( l, c ) {						\
      __cell_print( c, hl->h.out );					\
      csz+= c->csz;							\
      sz+= sizeof(*c);							\
      }									\
    fprintf( hl->h.out, "Content total: %d bytes", csz );		\
    fprintf( hl->h.out, " -- List total: %d bytes\n", csz+sz );		\
  } while ( 0 )							

#define __list_del_first( l, object ) do {				\
    cell c;								\
    if ( (l)->next->content )						\
      memcpy( &(object), (l)->next->content, sizeof( (object) ) );	\
    if ( (l)->next->content ) {						\
      free( (l)->next->content );					\
      c = (l)->next;							\
      (l)->next->next->prev = (l);					\
      (l)->next = (l)->next->next;					\
      free( c );							\
      __list_decr_count( l );						\
    }									\
  } while ( 0 ) 

#define __list_del_last( l, object ) do {				\
    cell c;								\
    memcpy( &(object), (l)->prev->content, sizeof( (object) ) );	\
    if ( (l)->prev->content ) {						\
      free( (l)->prev->content );					\
      c = (l)->prev;							\
      (l)->prev->prev->next = (l);					\
      (l)->prev = (l)->prev->prev;					\
      free( c );							\
      __list_decr_count( l );						\
    }									\
  } while ( 0 ) 

#define __list_del_first_object( l, object ) do {			\
    cell i = (l)->next;							\
    while ( i->content && memcmp( i->content, &(object), sizeof(object) ) ) \
      i = i->next;							\
    i = i->prev;							\
    if ( (i)->next->content ) {						\
      cell c;								\
      free( (i)->next->content );					\
      c = (i)->next;							\
      (i)->next->next->prev = (i);					\
      (i)->next = (i)->next->next;					\
      free( c );							\
      __list_decr_count( l );						\
    }									\
  } while ( 0 )

#define __list_del_all_object( l, object ) do {				\
    cell i = (l)->next;							\
    while ( i->content ) {						\
      if ( !memcmp( i->content, &(object), sizeof(object) ) ) {		\
	cell c;								\
	i = i->prev;							\
	free( (i)->next->content );					\
	c = i->next;							\
	(i)->next->next->prev = i;					\
	(i)->next = (i)->next->next;					\
	free( c );							\
	__list_decr_count( l );						\
      }									\
    i = i->next;							\
    }									\
  } while ( 0 )

#define __list_contains( l, object, found ) do {			\
    cell i = (l)->next;							\
    int f = -1;								\
    memcpy( &(found), &f, sizeof(f) );					\
    while ( i->content ) {						\
      f++;								\
      if ( !memcmp( i->content, &(object), sizeof(object) ) ) {		\
	memcpy( &(found), &f, sizeof(f) );				\
	break;								\
      }									\
      i = i->next;							\
    }									\
  } while ( 0 )

#define __list_delete( l ) do {						\
    unsigned int cnt;							\
    cell c;								\
    head hl = (head)( (char *)l-sizeof( (hl->h)) );			\
    __list_get_count( l, cnt );						\
    while( cnt ) {							\
      free( (l)->next->content );					\
      c = (l)->next;							\
      (l)->next->next->prev = (l);					\
      (l)->next = (l)->next->next;					\
      free( c );							\
      __list_decr_count( l );						\
      __list_get_count( l, cnt );					\
    }									\
    if ( hl->h.lname ) {						\
      free( hl->h.lname );						\
      hl->h.lname = NULL;						\
    }									\
    free( hl );								\
  } while( 0 ) 

#define __list_apply_function( l, function, type ) do {		\
    cell c;							\
    __list_foreach_forward( l, c ) {				\
      type t;							\
      memcpy( &t, (c)->content, sizeof( type ) );		\
      function( t );						\
    }								\
  } while ( 0 )


#define __list_compare(l1,l2,comp) do {		\
    unsigned int s1, s2;			\
    __list_get_count( l1, s1 );			\
    __list_get_count( l2, s2 );			\
    if ( s1 < s2 ) {				\
      s1 = -1;					\
      memcpy( &(comp), &s1, sizeof(comp) );	\
    }						\
    if ( s1 > s2 ) {				\
      s2 = 1;					\
      memcpy( &(comp), &s2, sizeof(comp) );	\
    }						\
    if ( s1==s2 ) {				\
      unsigned int t = 0;			\
      cell c1 = (l1)->next, c2 = (l2)->next;	\
      while ( (c1)->content ) {						\
	if ( (c1)->csz == (c2)->csz && !memcmp((c1)->content,(c2)->content, (c1)->csz)) { \
	  c1 = c1->next;						\
	  c2 = c2->next;						\
	}								\
	else {								\
	  t = 1;							\
	  break;							\
	}								\
      }									\
      memcpy( &(comp), &t, sizeof(t) );					\
    }									\
  } while ( 0 )

#define __list_append( l1, l2 ) do {		\
    head hl1 = __get_header(l1);		\
    head hl2 = __get_header(l2);		\
    l1->prev->next = l2->next;			\
    l2->next->prev = l1->prev;			\
    l2->prev->next = l1;			\
    hl1->h.count += hl1->h.count;		\
    l2 = &(hl2->data);				\
    if ( hl2->h.lname ) free( hl2->h.lname );	\
    free( hl2 );				\
  } while ( 0 ) 

#define __list_fusion( l1, l2 ) do {					\
    cell cc;								\
    __list_foreach( l2, cc ) {						\
      unsigned int prst = 0;						\
      cell c;								\
      __list_foreach( l1, c ) {						\
	if ( !memcmp( cc->content, c->content, sizeof(c->csz) ) )	\
	  prst = 1;							\
      }									\
      if ( !prst ) {							\
	cell t;								\
	__new_cell( t );						\
	t->content = malloc( cc->csz );					\
	t->csz = cc->csz;						\
	t->prev = (l1)->prev;						\
	t->next = (l1);							\
	(l1)->prev->next = t;						\
	(l1)->prev = t;							\
	memcpy( t->content, cc->content, cc->csz );			\
	__list_incr_count(l1);						\
      }									\
    }									\
  } while ( 0 )

#define __list_copy( l1, l2 ) do {					\
      cell i;								\
      head hl2 = __get_header( l2 );					\
      __list_init( l1 );						\
      __list_foreach( l2, i ) {						\
	cell c;								\
	__new_cell( c );						\
	c->prev = (l1)->prev;						\
	c->next = (l1);							\
	(l1)->prev->next = c;						\
	(l1)->prev = c;							\
	c->content = malloc( i->csz );					\
	c->csz = i->csz;						\
	memcpy( c->content, i->content, c->csz );			\
      }									\
      __list_set_count( l1, hl2->h.count );				\
      if ( hl2->h.lname ) __list_set_name( l1, hl2->h.lname );		\
  } while ( 0 )

#define __list_insert_once( l,object ) do {				\
    unsigned int prst = 0;						\
    cell c;								\
    __list_foreach( l, c ) {						\
      if ( !memcmp( &(object), c->content, sizeof(object) ) && sizeof(object)==c->csz ) \
	prst = 1;							\
    }									\
    if ( !prst ) __list_add_first( l, object );				\
  } while ( 0 )

#define __list_only_once( l ) do {					\
    list r;								\
    cell c;								\
    unsigned int cnt;							\
    __list_get_count( l, cnt );						\
    if ( cnt < 2 ) break;						\
    __list_init( r );							\
    __new_cell( c );							\
    c->next = r->next;							\
    c->prev = r;							\
    r->next->prev = c;							\
    r->next = c;							\
    c->content = malloc( l->next->csz );				\
    c->csz = l->next->csz;						\
    memcpy( c->content, l->next->content, l->next->csz );		\
    __list_incr_count( r );						\
    __list_foreach( l, c ) {						\
      cell tt;								\
      unsigned int prsnt = 0;						\
      __list_foreach( r, tt ) {						\
	if ( !memcmp( tt->content, c->content, c->csz ) && tt->csz==c->csz ) \
	  prsnt = 1;							\
      }									\
      if ( !prsnt ) {							\
	cell t;								\
	__new_cell( t );						\
	t->next = r->next;						\
	t->prev = r;							\
	r->next->prev = t;						\
	r->next = t;							\
	t->content = malloc( c->csz );					\
	t->csz = c->csz;						\
	memcpy( t->content, c->content, c->csz );			\
	__list_incr_count( r );						\
      }									\
    }								        \
    __list_delete( l );							\
    l = r;								\
  } while ( 0 )

#define __list_minus(l1,l2) do {					\
    cell c;								\
    list cl1, cl2;							\
    list r;								\
    __list_init( r );							\
    __list_copy( cl1, l1 );						\
    __list_copy( cl2, l2 );						\
    __list_only_once( cl1 );						\
    __list_only_once( cl2 );						\
    __list_foreach(cl2,c) {						\
      cell cc;								\
      __list_foreach( cl1,cc ) {					\
	if ( !memcmp(c->content, cc->content, c->csz) && c->csz==cc->csz ) { \
	  cell t;							\
	  __new_cell( t );						\
	  t->next = r->next;						\
	  t->prev = r;							\
	  r->next->prev = t;						\
	  r->next = t;							\
	  t->content = malloc( c->csz );				\
	  t->csz = c->csz;						\
	  memcpy( t->content, c->content, c->csz );			\
	  __list_incr_count( r );					\
	}								\
      }									\
    }									\
    __list_delete( l1 );						\
    __list_delete( cl2 );						\
    __list_delete( cl1 );						\
    l1 = r;								\
  } while ( 0 )
    
  /**************************************************************************************/



#define list_init() ({ list l; __list_init(l); l; })
#define list_set_name(l,name) __list_set_name(l,name)
#define list_get_count(l) ({ int cnt; __list_get_count(l, cnt); cnt; })
#define list_set_count(l,cnt) __list_set_count(l, cnt)
#define list_set_output(l,file) __list_set_output(l,file)
#define list_is_empty(l) ({ int empty; __list_is_empty(l,empty); empty;})
#define list_add_first(l,object) __list_add_first(l,object)
#define list_add_last(l,object) __list_add_last(l,object)
#define list_add_by_index(l,object,index) __list_add_by_index(l,object,index)
#define list_add_by_index_reverse(l,object,index) __list_add_by_index_reverse(l,object,index)
#define list_del_first_by_index(l,object,index) __list_del_first_by_index(l,object,index)
#define list_del_all_by_index(l,object,index) __list_del_all_by_index(l,object,index)
#define list_foreach(l,i) __list_foreach(l,i)
#define list_foreach_reverse(l,i) __list_foreach_reverse(l,i)
#define list_print(l) __list_print(l)
#define list_del_first(l,object) __list_del_first(l,object)
#define list_del_last(l,object) __list_del_last(l,object)
#define list_delete(l) __list_delete(l)
#define list_apply_function(l,function,type) __list_apply_function(l,function,type)
#define list_compare(l1,l2,comp) __list_compare(l1,l2,comp)
#define list_del_first_object(l,object) __list_del_first_object(l,object)
#define list_del_all_object(l,object) __list_del_all_object(l,object)
#define list_contains(l,object,found) __list_contains(l,object,found)
#define list_append(l1,l2) __list_append(l1,l2)
#define list_fusion(l1,l2) __list_fusion(l1,l2)
#define list_copy(l1,l2) __list_copy(l1,l2)
#define list_insert_once(l,object) __list_insert_once(l,object)
#define list_only_once(l) __list_only_once(l)

#define fifo_init(f) __list_init(f)
#define fifo_push(f,object) __list_add_first(f,object)
#define fifo_pop(f,object) __list_del_last(f,object)
#define fifo_delete(f) __list_delete(f)

#define lifo_init(l) __list_init(l)
#define lifo_push(l,object) __list_add_first(l,object)
#define lifo_pop(l,object) __list_del_first(l,object)
#define lifo_delete(l) __list_delete(l) 

#define set_init(s) __list_init(s) 
#define set_add(s,object) __list_insert_once(s,object) 
#define set_remove(s,object) __list_del_first_object(s,object)
#define set_delete(s) __list_delete(s) 
#define set_foreach(s,e) __list_foreach(s,e)
#define set_union(s1,s2) __list_fusion(s1,s2) 
#define set_inter(s1,s2) __list_minus(s1,s2)

  /**************************************************************************************/

typedef struct hash_header_t {
  char * hname; 
  FILE * out; 
  unsigned int count; 
} hash_header; 

typedef struct hash_t {
  hash_header h; 
  list * data; 
} * hash; 

#define __hash_init( h ) do {					\
    unsigned int i;						\
    (h) = malloc( sizeof( *(h) ) );				\
    (h)->h.count = 256;						\
    (h)->h.out = __TRICKLIST_OUTPUT;				\
    (h)->h.hname = NULL;					\
    (h)->data = malloc( 256*sizeof(struct cell_t) );		\
    for ( i= 0; i< 256; i++ ) list_init( (h)->data[i] );	\
  } while ( 0 )

#define __hash_set_output( l, file ) do {		\
    fprintf( stderr, "Warning: Changing output!\n" );	\
    (h)->h.out = (file);				\
  } while( 0 )

#define __hash_set_name( h, newname ) do {				\
    unsigned int i, len;						\
    len = strlen(newname)+1<__TRICKLIST_MAXLEN?strlen(newname):__TRICKLIST_MAXLEN; \
    if ( (h)->h.hname )							\
      {									\
	fprintf( stderr, "Warning: Changing name!\n" );			\
	free( (h)->h.hname );						\
      }									\
    (h)->h.hname = malloc( (1+strlen(newname))*sizeof(char) );		\
    for ( i= 0; i< len; i++ ) (h)->h.hname[i] = newname[i];		\
    (h)->h.hname[i] = '\0';						\
  } while ( 0 )								\

#define __hash_delete( h ) do {			\
    unsigned int i;				\
    for ( i= 0; i< (h)->h.count; i++ )		\
      list_delete( (h)->data[i] );		\
    if ( (h)->h.hname ) free( (h)->h.hname );	\
    free( h );					\
  } while ( 0 ) 

#define __hash_print( h ) do {						\
    unsigned int i;							\
    if ( (h)->h.hname )							\
      fprintf( (h)->h.out, "Hash %s @ %p ", (h)->h.hname, (void*)(h) );	\
    else								\
      fprintf( (h)->h.out, "Hash @ %p ", (void*)(h) );			\
    fprintf( (h)->h.out, "(%d buckets)\n", (h)->h.count );		\
    for ( i= 0; i< (h)->h.count; i++ )					\
      list_print( (h)->data[i] );					\
  } while ( 0 ) 

#define __hash_bytes( data, start, len, h ) do {			\
    unsigned int i;							\
    unsigned char * p, res;						\
    unsigned char T[256] = { 77, 175, 85, 225, 174, 222, 120, 192, 66, 233, 198, 7, 116, 227, 229, 15, 248, 68, 99, 156, 195, 254, 98, 148, 164, 177, 171, 196, 124, 8, 224, 179, 72, 191, 137, 5, 65, 31, 226, 106, 144, 142, 113, 205, 208, 219, 97, 63, 131, 34, 30, 193, 223, 41, 44, 145, 12, 215, 178, 154, 251, 181, 19, 117, 230, 82, 52, 188, 58, 108, 92, 55, 36, 47, 10, 20, 221, 139, 109, 57, 236, 59, 46, 134, 93, 35, 146, 190, 252, 105, 169, 38, 228, 234, 189, 199, 180, 138, 115, 152, 13, 214, 173, 79, 111, 23, 73, 112, 232, 182, 75, 28, 141, 172, 122, 110, 130, 86, 2, 201, 14, 114, 140, 125, 95, 161, 220, 184, 162, 81, 119, 241, 153, 87, 69, 247, 48, 200, 94, 70, 88, 61, 255, 103, 136, 32, 27, 11, 202, 194, 17, 107, 235, 118, 22, 26, 91, 3, 209, 186, 100, 24, 183, 104, 167, 123, 207, 42, 90, 143, 217, 6, 4, 9, 132, 101, 237, 150, 168, 206, 102, 29, 80, 155, 62, 187, 51, 231, 64, 33, 239, 128, 163, 74, 43, 157, 253, 165, 160, 50, 54, 204, 147, 53, 16, 197, 121, 170, 216, 84, 151, 45, 21, 240, 127, 211, 56, 89, 40, 129, 166, 1, 133, 245, 76, 238, 176, 159, 18, 0, 243, 96, 158, 185, 149, 37, 49, 67, 135, 25, 218, 242, 83, 246, 249, 39, 250, 126, 213, 203, 78, 212, 210, 71, 60 }; \
    for ( res = 0, i= 0, p = ((unsigned char *)data)+start; i< len; res = T[res^p[i++]] ); \
    memcpy( &(h), &res, sizeof(unsigned char) );			\
  } while ( 0 )

#define __hash_object( object, hash ) __hash_bytes( &(object), 0, sizeof(object), hash )

#define __hash_from_key( object, key, hash ) __hash_bytes( &((object).key), (((char*)&((object).key))-((char*)&(object))), sizeof((object).key), hash )

#define __hash_insert_object( h, object ) do {		\
    unsigned char hash=0;				\
    __hash_object( object, hash );			\
    list_add_first( (h)->data[hash], object );		\
  } while ( 0 ) 

#define __hash_remove_object( h, object ) do {		\
    unsigned char hash=0;				\
    __hash_object( object, hash );			\
    list_del_all_object((h)->data[hash],object);	\
  } while ( 0 ) 

#define __hash_lookup_object( h, object, found ) do {	\
    unsigned char hash=0;				\
    __hash_object( object, hash );			\
    list_contains((h)->data[hash],object,found);	\
  } while ( 0 ) 

#define __hash_insert_from_key( h, object, key ) do {	\
    unsigned char hash=0;				\
    __hash_from_key( object, key, hash );		\
    list_add_first( (h)->data[hash], object );		\
  } while ( 0 ) 

#define __hash_remove_from_key( h, object, key ) do {	\
    unsigned char hash=0;				\
    __hash_from_key( object, key, hash );		\
    list_del_all_object((h)->data[hash],object);	\
  } while ( 0 ) 

#define __hash_lookup_from_key( h, object, key, found ) do {	\
    unsigned char hash=0;					\
    __hash_from_key( object, key, hash );			\
    list_contains((h)->data[hash],object,found);		\
  } while ( 0 ) 

#define hash_init(h) __hash_init(h) 
#define hash_set_name(h,name) __hash_set_name(h,name)
#define hash_set_output(h,out) __hash_set_name(h,out)
#define hash_print(h) __hash_print(h) 
#define hash_insert_object(h,object) __hash_insert_object(h,object)
#define hash_remove_object(h,object) __hash_remove_object(h,object)
#define hash_lookup_object(h,object,found) __hash_lookup_object(h,object,found)
#define hash_insert_from_key(h,object,key) __hash_insert_from_key(h,object,key)
#define hash_remove_from_key(h,object,key) __hash_remove_from_key(h,object,key)
#define hash_lookup_from_key(h,object,key,found) __hash_lookup_from_key(h,object,key,found)
#define hash_delete(h) __hash_delete(h)


#ifdef __cplusplus
}
#endif

#endif
