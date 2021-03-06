
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "dpair.h"


/* pair_container: */

typedef struct
{
    container	*A, *B;
} pair_container_priv;


int pair_compare( container *C, value a, value b )
{
    int		ret;

    ret = compare( ((pair_container_priv*)C->priv)->A, pair(a).first, pair(b).first );

    if (ret != 0)
	return ret;

    return compare( ((pair_container_priv*)C->priv)->B, pair(a).second, pair(b).second );
}

alloc_data pair_ap_allocate( container *C, va_list ap )
{
    alloc_data	zap;
    value	v, a, b;
    container	*A, *B;
    alloc_data	ad;

    A = ((pair_container_priv*)C->priv)->A;
    B = ((pair_container_priv*)C->priv)->B;

    ad = A->ap_allocate(A, ap);
    a = ad.v;
    ap = ad.ap;

    ad = B->ap_allocate(B, ap);
    b = ad.v;
    ap = ad.ap;

    v.ptr = (void*)malloc(sizeof(pair_value));
    ((pair_value*)v.ptr)->first = a;
    ((pair_value*)v.ptr)->second = b;

    zap.v = v;
    zap.ap = ap;

    return zap;
}

void pair_deallocate( container *C, value v )
{
    value	a, b;
    container	*A, *B;

    a = ((pair_value*)v.ptr)->first;
    b = ((pair_value*)v.ptr)->second;

    A = ((pair_container_priv*)C->priv)->A;
    B = ((pair_container_priv*)C->priv)->B;
    A->deallocate(A, a);
    B->deallocate(B, b);

    free(v.ptr);
}

void pair_destroy( container *C )
{
    container	*A = ((pair_container_priv*)C->priv)->A,
		*B = ((pair_container_priv*)C->priv)->B;

    destroy( A );
    destroy( B );

    free(C->priv);
    free(C);
}

container* pair_container( container *A, container *B )
{
    container	*C = (container*)malloc(sizeof(container));

    C->compare = pair_compare;
    C->ap_allocate = pair_ap_allocate;
    C->deallocate = pair_deallocate;
    C->destroy = pair_destroy;
    C->priv = (void*)malloc(sizeof(pair_container_priv));
    ((pair_container_priv*)C->priv)->A = A;
    ((pair_container_priv*)C->priv)->B = B;

    return C;
}

