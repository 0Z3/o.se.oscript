#include <string.h>

#include "ose_conf.h"
#include "ose.h"
#include "ose_context.h"
#include "ose_util.h"
#include "ose_stackops.h"
#include "ose_assert.h"
#include "ose_vm.h"
#include "ose_print.h"
#include "ose_errno.h"
#include "ose_builtins.h"

#include "oscript_types.h"

void oscript_getPayloadItemLength(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char typetag = (char)ose_popInt32(vm_s);
    int32_t offset = ose_popInt32(vm_s);
    switch(typetag)
    {
    /* case OSCRIPT_TYPETAG_FUNCALL: */
    /* case OSCRIPT_TYPETAG_ASSIGN: */
    case OSCRIPT_TYPETAG_LAMBDA:
    case OSETT_BUNDLE:
        ose_pushInt32(vm_s, ose_readInt32(vm_s, offset));
        break;
    /* case OSCRIPT_TYPETAG_LOOKUP: */
    /*     ose_pushInt32(vm_s, */
    /*                   strlen(ose_getBundlePtr(vm_s) + offset)); */
        break;
    }
}

void oscript_getPayloadItemSize(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char typetag = (char)ose_popInt32(vm_s);
    int32_t offset = ose_popInt32(vm_s);
    switch(typetag)
    {
    /* case OSCRIPT_TYPETAG_FUNCALL: */
    /* case OSCRIPT_TYPETAG_ASSIGN: */
    case OSCRIPT_TYPETAG_LAMBDA:
    case OSETT_BUNDLE:
        ose_pushInt32(vm_s,
                      ose_getPaddedBlobSize(vm_s, offset) + 4);
        break;
    /* case OSCRIPT_TYPETAG_LOOKUP: */
    /*     ose_pushInt32(vm_s, */
    /*                   ose_pstrlen(ose_getBundlePtr(vm_s) */
    /*                               + offset)); */
        break;
    }
}

void oscript_pprintPayloadItem(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char typetag = (char)ose_popInt32(vm_s);
    int32_t offset = ose_popInt32(vm_s);
    (void)offset; // silence compiler warning about unused var
    switch(typetag)
    {
    /* case OSCRIPT_TYPETAG_ASSIGN: */
        /* { */
        /*     int32_t n = ose_pprintBundleElem_msg(vm_s, */
        /*                                          offset, NULL, 0, 0); */
        /*     int32_t pn = ose_pnbytes(n); */
        /*     ose_pushBlob(vm_s, pn, NULL); */
        /*     char *p = ose_peekBlob(vm_s) + 4; */
        /*     ose_pprintBundleElem_msg(vm_s, offset, p, pn, 0); */
        /*     ose_pushInt32(vm_s, OSETT_STRING); */
        /*     ose_blobToType(vm_s); */
        /*     ose_pushInt32(vm_s, n); */
        /* } */
        /* ose_assert(0); */
    /*     break; */
    /* case OSCRIPT_TYPETAG_LOOKUP: */
        /* { */
    /*         const char * const str = ose_readString(vm_s, offset); */
    /*         ose_pushString(vm_s, str); */
    /*         ose_pushInt32(vm_s, strlen(str)); */
    /*     } */
    /*     ose_assert(0); */
    /*     break; */
    /* case OSCRIPT_TYPETAG_FUNCALL: */
        /* { */
        /*     ose_pushString(vm_s, "<!>"); */
        /*     ose_pushInt32(vm_s, 3); */
        /* } */
        /* ose_assert(0); */
        /* break; */
    case OSCRIPT_TYPETAG_LAMBDA:
        {
            ose_pushString(vm_s, "<lambda>");
            ose_pushInt32(vm_s, strlen("<lambda>"));
        }
        break;
    case OSETT_BUNDLE:
        ose_pushString(vm_s, "<bndl>");
        ose_pushInt32(vm_s, 6);
        break;
    }
}

void oscript_evalType(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    if(ose_peekType(vm_s) == OSETT_BUNDLE)
    {
        ose_pushString(vm_c, "/!/o/exec");
        ose_swap(vm_c);
    }
    else
    {
        const char tt = ose_peekMessageArgType(vm_s);
        switch(tt)
        {
            /* case OSCRIPT_TYPETAG_LOOKUP: */
            /*     ose_itemToBlob(vm_s); */
            /*     ose_pushInt32(vm_s, (int32_t)OSETT_STRING); */
            /*     ose_blobToType(vm_s); */
            /*     ose_builtin_lookupInEnv(osevm); */
            /*     ose_assert(0); */
            /*     break; */
            /* case OSCRIPT_TYPETAG_ASSIGN: */
            /*     ose_setTypetag_impl(vm_s, OSETT_BLOB); */
            /*     ose_blobToElem(vm_s); */
            /*     ose_pushBundle(vm_s); */
            /*     ose_swap(vm_s); */
            /*     ose_push(vm_s); */
            /*     ose_pushString(vm_s, "/!/o/assign"); */
            /*     ose_push(vm_s); */
        
            /*     ose_pushString(vm_c, "/</_e"); */
            /*     ose_swap(vm_c); */
            /*     ose_pushString(vm_c, "/!/exec1"); */
            /*     ose_swap(vm_c); */
            /*     ose_assert(0); */
            /*     break; */
            /* case OSCRIPT_TYPETAG_FUNCALL: */
            /*     ose_setTypetag_impl(vm_s, OSETT_BLOB); */
            /*     ose_blobToElem(vm_s); */
            /*     ose_unpackDrop(vm_s); */
            /*     { */
            /*         ose_drop(vm_c); */
            /*         ose_pushString(vm_c, "/!/o/apply"); */
            /*         ose_moveElem(vm_s, vm_c); */
            /*         // ose_pushString(vm_c, "/!/o/eval"); */
            /*         ose_pushString(vm_c, "/!/o/prepargs"); */
            /*         ose_moveElem(vm_s, vm_c); */
            /*         ose_pushMessage(vm_c, */
            /*                         OSE_ADDRESS_ANONVAL, */
            /*                         OSE_ADDRESS_ANONVAL_LEN, 0); */
            /*     } */
            /*     ose_assert(0); */
            /*     break; */
        case OSCRIPT_TYPETAG_LAMBDA:
            ;
            break;
        case OSETT_BUNDLE:
            ose_setTypetag_impl(vm_s, OSETT_BLOB);
            ose_blobToElem(vm_s);
            ose_pushString(vm_c, "/!/o/exec");
            ose_swap(vm_c);
            break;
        }
    }
}

const char * const
oscript_lambda_getParam(struct oscript_lambda_params p,
                        int32_t n)
{
    if(n >= p.nparams)
    {
        return NULL;
    }
    if(n < OSCRIPT_LAMBDA_PARAMS_CACHE_SIZE)
    {
        return p.params + p.cache[n];
    }
    else
    {
        char *ptr = (char *)p.params;
        for(int32_t i = 0; i < n; ++i)
        {
            ptr += ose_pstrlen(ptr);
        }
        return ptr;
    }
}

struct oscript_lambda_params
oscript_lambda_getParams(ose_bundle bundle,
                         int32_t offset)
{
    int32_t o = offset + 4 + OSE_BUNDLE_HEADER_LEN;
    o += ose_readInt32(bundle, o) + 4;
    const int32_t tto = o + 4
        + ose_pstrlen(ose_readString(bundle, o + 4));
    const int32_t ntt = strlen(ose_readString(bundle, tto)) - 1;
    const int32_t plo = tto + ose_pnbytes(ntt + 1);
    struct oscript_lambda_params p =
    {
        ntt, ose_getBundlePtr(bundle) + plo, {}
    };
    int32_t oo = plo;
    for(int i = 0;
        i < OSCRIPT_LAMBDA_PARAMS_CACHE_SIZE && i < ntt;
        ++i)
    {
        const char * const s = ose_readString(bundle, oo);
        p.cache[i] = oo - plo;
        oo += ose_pstrlen(s);
    }
    return p;
}

void oscript_types_load(ose_bundle vm_s)
{
    // Assignment
    // {
    //     char buf[OSEVM_EVALTYPE_ADDR_LEN];
    //     int32_t len = snprintf(buf, OSEVM_EVALTYPE_ADDR_LEN,
    //                            OSEVM_EVALTYPE_ADDR,
    //                            OSCRIPT_TYPETAG_ASSIGN);
    //     ose_pushMessage(vm_s, buf, len,
    //                     1, OSETT_ALIGNEDPTR, oscript_evalType);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_ASSIGN_STR
    //                      "/getPayloadItemLength"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_ASSIGN_STR
    //                            "/getPayloadItemLength"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemLength);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_ASSIGN_STR
    //                      "/getPayloadItemSize"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_ASSIGN_STR
    //                            "/getPayloadItemSize"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemSize);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_ASSIGN_STR
    //                      "/pprintPayloadItem"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_ASSIGN_STR
    //                            "/pprintPayloadItem"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_pprintPayloadItem);
    //     ose_push(vm_s);
    // }

    // {
    //     // Lookup
    //     char buf[OSEVM_EVALTYPE_ADDR_LEN];
    //     int32_t len = snprintf(buf, OSEVM_EVALTYPE_ADDR_LEN,
    //                            OSEVM_EVALTYPE_ADDR,
    //                            OSCRIPT_TYPETAG_LOOKUP);
    //     ose_pushMessage(vm_s, buf, len,
    //                     1, OSETT_ALIGNEDPTR, oscript_evalType);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_LOOKUP_STR
    //                      "/getPayloadItemLength"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_LOOKUP_STR
    //                            "/getPayloadItemLength"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemLength);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_LOOKUP_STR
    //                      "/getPayloadItemSize"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_LOOKUP_STR
    //                            "/getPayloadItemSize"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemSize);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_LOOKUP_STR
    //                      "/pprintPayloadItem"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_LOOKUP_STR
    //                            "/pprintPayloadItem"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_pprintPayloadItem);
    //     ose_push(vm_s);
    // }

    // {
    //     // Funcall
    //     char buf[OSEVM_EVALTYPE_ADDR_LEN];
    //     int32_t len = snprintf(buf, OSEVM_EVALTYPE_ADDR_LEN,
    //                            OSEVM_EVALTYPE_ADDR,
    //                            OSCRIPT_TYPETAG_FUNCALL);
    //     ose_pushMessage(vm_s, buf, len,
    //                     1, OSETT_ALIGNEDPTR, oscript_evalType);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_FUNCALL_STR
    //                      "/getPayloadItemLength"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_FUNCALL_STR
    //                            "/getPayloadItemLength"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemLength);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_FUNCALL_STR
    //                      "/getPayloadItemSize"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_FUNCALL_STR
    //                            "/getPayloadItemSize"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_getPayloadItemSize);
    //     ose_push(vm_s);
    //     ose_pushMessage(vm_s,
    //                     ("/type/"
    //                      OSCRIPT_TYPETAG_FUNCALL_STR
    //                      "/pprintPayloadItem"),
    //                     strlen("/type/"
    //                            OSCRIPT_TYPETAG_FUNCALL_STR
    //                            "/pprintPayloadItem"),
    //                     1, OSETT_ALIGNEDPTR,
    //                     oscript_pprintPayloadItem);
    //     ose_push(vm_s);
    // }

    {
        // Lambda
        char buf[OSEVM_EVALTYPE_ADDR_LEN];
        int32_t len = snprintf(buf, OSEVM_EVALTYPE_ADDR_LEN,
                               OSEVM_EVALTYPE_ADDR,
                               OSCRIPT_TYPETAG_LAMBDA);
        ose_pushMessage(vm_s, buf, len,
                        1, OSETT_ALIGNEDPTR, oscript_evalType);
        // ose_push(vm_s);
        ose_pushMessage(vm_s,
                        ("/type/"
                         OSCRIPT_TYPETAG_LAMBDA_STR
                         "/getPayloadItemLength"),
                        strlen("/type/"
                               OSCRIPT_TYPETAG_LAMBDA_STR
                               "/getPayloadItemLength"),
                        1, OSETT_ALIGNEDPTR,
                        oscript_getPayloadItemLength);
        // ose_push(vm_s);
        ose_pushMessage(vm_s,
                        ("/type/"
                         OSCRIPT_TYPETAG_LAMBDA_STR
                         "/getPayloadItemSize"),
                        strlen("/type/"
                               OSCRIPT_TYPETAG_LAMBDA_STR
                               "/getPayloadItemSize"),
                        1, OSETT_ALIGNEDPTR,
                        oscript_getPayloadItemSize);
        // ose_push(vm_s);
        ose_pushMessage(vm_s,
                        ("/type/"
                         OSCRIPT_TYPETAG_LAMBDA_STR
                         "/pprintPayloadItem"),
                        strlen("/type/"
                               OSCRIPT_TYPETAG_LAMBDA_STR
                               "/pprintPayloadItem"),
                        1, OSETT_ALIGNEDPTR,
                        oscript_pprintPayloadItem);
        // ose_push(vm_s);
    }

    {
        // Sub-bundle
        char buf[256];
        int32_t len = snprintf(buf, 256,
                               OSEVM_EVALTYPE_ADDR,
                               OSETT_BUNDLE);
        ose_pushMessage(vm_s, buf, len,
                        1, OSETT_ALIGNEDPTR, oscript_evalType);
        // ose_push(vm_s);

        len = snprintf(buf, 256,
                       OSE_GETPAYLOADITEMSIZE_ADDR,
                       OSETT_BUNDLE);
        ose_pushMessage(vm_s, buf, len,
                        1, OSETT_ALIGNEDPTR,
                        oscript_getPayloadItemSize);
        // ose_push(vm_s);

        len = snprintf(buf, 256,
                       OSE_GETPAYLOADITEMLENGTH_ADDR,
                       OSETT_BUNDLE);
        ose_pushMessage(vm_s, buf, len,
                        1, OSETT_ALIGNEDPTR,
                        oscript_getPayloadItemLength);
        // ose_push(vm_s);

        len = snprintf(buf, 256,
                       OSE_PPRINTPAYLOADITEM_ADDR,
                       OSETT_BUNDLE);
        ose_pushMessage(vm_s, buf, len,
                        1, OSETT_ALIGNEDPTR,
                        oscript_pprintPayloadItem);
        // ose_push(vm_s);
        
    }
}
