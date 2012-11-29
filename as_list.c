#include "as_list.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const as_val AS_LIST_VAL;


as_list * as_list_new(void * source, const as_list_hooks * hooks) {
    as_list * l = (as_list *) malloc(sizeof(as_list));
    l->_ = AS_LIST_VAL;
    l->source = source;
    l->hooks = hooks;
    return l;
}

int as_list_free(as_list * l) {
    if ( l->hooks == NULL ) return 1;
    if ( l->hooks->free == NULL ) return 2;
    return l->hooks->free(l);
}

static uint32_t as_list_hash(as_list * l) {
    if ( l->hooks == NULL ) return 0;
    if ( l->hooks->hash == NULL ) return 0;
    return l->hooks->hash(l);
}

void * as_list_source(const as_list * l) {
    return l->source;
}

uint32_t as_list_size(as_list * l) {
    if ( l->hooks == NULL ) return 0;
    if ( l->hooks->size == NULL ) return 0;
    return l->hooks->size(l);
}

int as_list_append(as_list * l, as_val * v) {
    if ( l->hooks == NULL ) return 1;
    if ( l->hooks->append == NULL ) return 2;
    return l->hooks->append(l, v);
}

int as_list_prepend(as_list * l, as_val * v) {
    if ( l->hooks == NULL ) return 1;
    if ( l->hooks->prepend == NULL ) return 2;
    return l->hooks->prepend(l, v);
}

as_val * as_list_get(const as_list * l, const uint32_t i) {
    if ( l->hooks == NULL ) return NULL;
    if ( l->hooks->get == NULL ) return NULL;
    return l->hooks->get(l, i);
}

int as_list_set(as_list * l, const uint32_t i, as_val * v) {
    if ( l->hooks == NULL ) return 1;
    if ( l->hooks->set == NULL ) return 2;
    return l->hooks->set(l, i, v);
}

as_val * as_list_head(const as_list * l) {
    if ( l->hooks == NULL ) return NULL;
    if ( l->hooks->head == NULL ) return NULL;
    return l->hooks->head(l);
}

as_list * as_list_tail(const as_list * l) {
    if ( l->hooks == NULL ) return NULL;
    if ( l->hooks->tail == NULL ) return NULL;
    return l->hooks->tail(l);
}

as_iterator * as_list_iterator(const as_list * l) {
    if ( l->hooks == NULL ) return NULL;
    if ( l->hooks->iterator == NULL ) return NULL;
    return l->hooks->iterator(l);
}

as_val * as_list_toval(const as_list * l) {
    return (as_val *) l;
}

as_list * as_list_fromval(const as_val * v) {
    return as_val_type(v) == AS_LIST ? (as_list *) v : NULL;
}

static int as_list_val_free(as_val * v) {
    return as_val_type(v) == AS_LIST ? as_list_free((as_list *) v) : 1;
}

static uint32_t as_list_val_hash(as_val * v) {
    return as_val_type(v) == AS_INTEGER ? as_list_hash((as_list *) v) : 0;
}

static char * as_list_val_tostring(as_val * v) {
    if ( as_val_type(v) != AS_LIST ) return NULL;

    as_list * l = (as_list *) v;

    char *      buf = NULL;
    uint32_t    blk = 256;
    uint32_t    cap = blk;
    uint32_t    pos = 0;
    bool        sep = false;

    buf = (char *) malloc(sizeof(char) * cap);
    bzero(buf, sizeof(char) * cap);

    strcpy(buf, "List(");
    pos += 5;
    
    as_iterator * i = as_list_iterator(l);
    while ( as_iterator_has_next(i) ) {
        as_val * val = as_iterator_next(i);
        char * valstr = as_val_tostring(val);
        size_t vallen = strlen(valstr);

        if ( pos + vallen + 2 >= cap ) {
            uint32_t adj = vallen+2 > blk ? vallen+2 : blk;
            buf = realloc(buf, sizeof(char) * (cap + adj));
            bzero(buf+cap, sizeof(char)*adj);
            cap += adj;
        }

        if ( sep ) {
            strcpy(buf + pos, ", ");
            pos += 2;
        }

        strncpy(buf + pos, valstr, vallen);
        pos += vallen;
        sep = true;
    }

    strcpy(buf + pos, ")");
    
    return buf;
}

static const as_val AS_LIST_VAL = {
    .type       = AS_LIST, 
    .free       = as_list_val_free, 
    .hash       = as_list_val_hash, 
    .tostring   = as_list_val_tostring
};