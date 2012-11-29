#include "as_boolean.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct as_boolean_s {
    as_val _;
    bool value;
};


static const as_val AS_BOOLEAN_VAL;
static int as_boolean_freeval(as_val *);


as_boolean * as_boolean_new(bool b) {
    as_boolean * v = (as_boolean *) malloc(sizeof(as_boolean));
    v->_ = AS_BOOLEAN_VAL;
    v->value = b;
    return v;
}

int as_boolean_free(as_boolean * b) {
    free(b);
    return 0;
}

uint32_t as_boolean_hash(const as_boolean * b) {
    return b->value ? 1 : 0;
}

bool as_boolean_tobool(const as_boolean * b) {
    return b->value;
}

as_val * as_boolean_toval(const as_boolean * b) {
    return (as_val *) b;
}

as_boolean * as_boolean_fromval(const as_val * v) {
    return as_val_type(v) == AS_BOOLEAN ? (as_boolean *) v : NULL;
}

static int as_boolean_val_free(as_val * v) {
    return as_val_type(v) == AS_BOOLEAN ? as_boolean_free((as_boolean *) v) : 1;
}

static uint32_t as_boolean_val_hash(as_val * v) {
    return as_val_type(v) == AS_BOOLEAN ? as_boolean_hash((as_boolean *) v) : 0;
}

static char * as_boolean_val_tostring(as_val * v) {
    if ( as_val_type(v) != AS_BOOLEAN ) return NULL;

    as_boolean * b = (as_boolean *) v;
    char * str = (char *) malloc(sizeof(char) * 6);
    bzero(str,6);
    if ( b->value ) {
        strcpy(str,"true");
    }
    else {
        strcpy(str,"false");
    }
    return str;

}

static const as_val AS_BOOLEAN_VAL = {
    .type       = AS_BOOLEAN, 
    .free       = as_boolean_val_free, 
    .hash       = as_boolean_val_hash,
    .tostring   = as_boolean_val_tostring
};