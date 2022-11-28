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

#define OSCRIPT_LAMBDA_PARAMS_CACHE_SIZE 12

struct oscript_lambda_params
{
    int32_t nparams;
    const char * const params;
    int32_t cache[OSCRIPT_LAMBDA_PARAMS_CACHE_SIZE];
};

const char * const
oscript_lambda_getParam(struct oscript_lambda_params p,
                        int32_t n);
struct oscript_lambda_params
oscript_lambda_getParams(ose_bundle bundle,
                         int32_t offset);

void oscript_types_load(ose_bundle);

#ifdef __cplusplus
}
#endif

#endif
