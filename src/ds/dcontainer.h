
/**
 *	(C) Copyright 2006-2010, Magnus Gal�en
 *
 *	dcontainer.h: Basic containers.
 */

#ifndef _DCONTAINER_H_
#define _DCONTAINER_H_

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdarg.h>
#include <string.h>
#include "../common/bprint.h"

//#define ex(_type,_var) (*((_type##_value*)_var.ptr))

/*
    La oss kalle datastrukturer for containere.
    Containere skal kunne inneholde andre containere.
    Maa ha allocate og deallocate.
    Og compare.
*/

typedef struct container container;
typedef struct iterator iterator;
typedef struct iterator2 iterator2;

typedef union
{
    int		i;
    char	c;
    double	d;
    void	*ptr;
    char	*str;
    container	*C;
} value;

typedef struct alloc_data
{
    value	v;
    va_list	ap;
} alloc_data;


struct container
{
    int		(*compare)( container *C, value a, value b );
    alloc_data	(*ap_allocate)( container *C, va_list ap );
    void	(*deallocate)( container *C, value a );
    void	(*destroy)( container *C );
    void	(*clear)( container *C );
    container*	(*clone)( container *C );
    value	(*copy)( container *C, value a );
    void	(*print)( container *C, value a );
    void	(*bprint)( container *C, buffer *B, char *delim, value a );
    void	*priv;
    void	*childC;
};

struct iterator
{
    void	*node;
    int		valid;
};

struct iterator2
{
    void	*node;
    int		valid;
    iterator2	(*next)( iterator2 );
    value	(*get_key)( iterator2 );
    value	(*get_value)( iterator2 );
    int		(*compare_keys)( container *C, value a, value b );
    container	*C;
    void	(*insert)( container *C, value v );
};

#define ds_next(it2) (it2 = it2.next(it2))
#define ds_key(it2) (it2.get_key(it2))
#define ds_value(it2) (it2.get_value(it2))

/* fancy allocate: */

int ds_compare( container *C, value a, value b );

value allocate( container *C, ... );

void deallocate( container *C, value v );

void destroy( container *C );

void clear( container *C );

void print( container *C );
void println( container *C );
void printv( container *C, value v );
char* asprint( container *C, char *delim );
void bprintv( container *C, buffer *B, char *delim, value v );

static inline value copy( container *C, value v )
{
    return C->copy( C, v );
}

container* ds_intersection( iterator2 a, iterator2 b );
container* ds_union( iterator2 a, iterator2 b );
//void destroy_iterator( iterator *it );


/* int_container: */

container* int_container();

/* string_container: */

container* string_container();

container* ptr_container();

/* custom_container: */

//container* custom_container(int obj_size, int(*compare)(container*, value, value));


static inline value int_value( int val )
{
    value	v;
    v.i = val;
    return v;
}


static inline value string_value( char *val )
{
    value	v;
    v.ptr = strdup(val);
    return v;
}


static inline value ptr_value( void *val )
{
    value	v;
    v.ptr = val;
    return v;
}


static inline value container_value( container *val )
{
    value	v;
    v.C = val;
    return v;
}


#endif	// _DCONTAINER_H_
