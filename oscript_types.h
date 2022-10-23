#include "ose.h"

#ifndef OSCRIPT_TYPES_H
#define OSCRIPT_TYPES_H

/* #define OSCRIPT_TYPETAG_ASSIGN '@' */
/* #define OSCRIPT_TYPETAG_ASSIGN_STR "@" */
/* #define OSCRIPT_TYPETAG_LOOKUP '$' */
/* #define OSCRIPT_TYPETAG_LOOKUP_STR "$" */
/* #define OSCRIPT_TYPETAG_FUNCALL '!' */
/* #define OSCRIPT_TYPETAG_FUNCALL_STR "!" */
#define OSCRIPT_TYPETAG_LAMBDA '\\'
#define OSCRIPT_TYPETAG_LAMBDA_STR "\\"

#ifdef __cplusplus
extern "C" {
#endif

void oscript_types_load(ose_bundle);

#ifdef __cplusplus
}
#endif

#endif
