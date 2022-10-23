#include <string.h>

#include "ose_conf.h"
#include "ose.h"
#include "ose_context.h"
#include "ose_util.h"
#include "ose_stackops.h"
#include "ose_assert.h"
#include "ose_vm.h"
#include "ose_symtab.h"
#include "ose_print.h"
#include "ose_errno.h"
#include "ose_builtins.h"

#include "oscript_lib.h"
#include "oscript_types.h"

static void oscript_bindFnArgs(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_assert(!ose_bundleIsEmpty(vm_s));
    ose_assert(ose_peekType(vm_s) == OSETT_BUNDLE);
    char *b = ose_getBundlePtr(vm_s);
    const int32_t argbndl_o = ose_getLastBundleElemOffset(vm_s);
    {
        // this manipulates the bundle, but doesn't change its
        // position, so the offset remains valid
        ose_pop(vm_s);
        ose_swap(vm_s);
        ose_unpackDrop(vm_s);
        // ose_popAllDropBundle(vm_s);
        ose_swap(vm_s);
    }
    const int32_t argbndl_s = ose_readInt32(vm_s, argbndl_o);
    const int32_t addrmsg_o = argbndl_o + argbndl_s + 4;
    const int32_t addrmsg_tt_o = addrmsg_o + 4
        + ose_pstrlen(b + addrmsg_o + 4);
    const int32_t naddrs = strlen(b + addrmsg_tt_o) - 1;
    int32_t addr_o = addrmsg_tt_o + ose_pnbytes(naddrs + 1);
    int32_t argelem_o = argbndl_o + 4 + OSE_BUNDLE_HEADER_LEN;
    OSE_START_BUNDLE(vm_s);
    for(int32_t i = 0; i < naddrs; ++i)
    {
        const int32_t argelem_s = ose_readInt32(vm_s, argelem_o);
        const int32_t addrlen = strlen(b + addr_o);
        const int32_t paddrlen = ose_pnbytes(addrlen);
        
        if(!strcmp(b + argelem_o + 4, OSE_BUNDLE_ID))
        {
            ose_pushMessage(vm_s, b + addr_o, addrlen, 1,
                            OSETT_BLOB,
                            argelem_s, b + argelem_o + 4);
            ose_setTypetag_impl(vm_s, OSETT_BUNDLE);
        }
        else
        {
            const int32_t argelem_tt_o = argelem_o + 4
                + ose_pstrlen(b + argelem_o + 4);
            const int32_t nbytes = argelem_s
                - (argelem_tt_o - (argelem_o + 4));
            const int32_t oldsize = ose_readSize(vm_s);
            ose_pushMessage(vm_s, b + addr_o, addrlen, 0);
            const int32_t newsize = ose_readSize(vm_s);
            ose_addToSize(vm_s, nbytes - 4);
            // b + newsize - 4 because ose_pushMessage includes
            // typetags even if there's no data
            memcpy(b + newsize - 4, b + argelem_tt_o, nbytes);
            ose_writeInt32(vm_s, oldsize, paddrlen + nbytes);
        }
        addr_o += paddrlen;
        argelem_o += argelem_s + 4;
    }
    OSE_END_BUNDLE(vm_s);
    ose_notrot(vm_s);
    ose_2drop(vm_s);
}

static void oscript_applyLambda(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_setTypetag_impl(vm_s, OSETT_BLOB);
    ose_blobToElem(vm_s);
    ose_popAllDrop(vm_s);
    {
        if(ose_peekType(vm_s) == OSETT_MESSAGE)
        {
            char tt = ose_peekMessageArgType(vm_s);
            switch(tt)
            {
            case OSETT_BUNDLE:
                break;
            case OSETT_BLOB:
                ose_setTypetag_impl(vm_s, OSETT_BUNDLE);
                break;
            default:
                __builtin_debugtrap();
                break;
            }
        }
        else
        {
            __builtin_debugtrap();
        }
    }
    ose_bundleAll(vm_s);
    ose_pop(vm_s);
    ose_swap(vm_s);
    oscript_bindFnArgs(osevm);
    ose_unpackDrop(vm_s);
    ose_rollBottom(vm_s);
    ose_bundleAll(vm_s);

    ose_pushString(vm_c, "/!/o/execlambdaapp");
    ose_swap(vm_c);
}

static void oscript_apply(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    if(ose_getBundleElemCount(vm_s) == 1)
    {
        __builtin_debugtrap();        
        ose_pop(vm_s);
    }
    if(ose_peekType(vm_s) == OSETT_MESSAGE)
    {
        const char tt = ose_peekMessageArgType(vm_s);
        switch(tt)
        {
        case OSETT_BUNDLE:
            ose_setTypetag_impl(vm_s, OSETT_BLOB);
            ose_blobToElem(vm_s);
            break;
        case OSCRIPT_TYPETAG_LAMBDA:
            oscript_applyLambda(osevm);
            return;
            break;
        }
    }
    if(ose_peekType(vm_s) == OSETT_BUNDLE)
    {
        // if this is a bundle with a single bundle
        // inside, it's probably a thunk and should
        // be unpacked
        int32_t o = ose_getLastBundleElemOffset(vm_s);
        int32_t s = ose_readInt32(vm_s, o);
        const char * const b = ose_getBundlePtr(vm_s);
        if(s >= (OSE_BUNDLE_HEADER_LEN * 2) + 4)
        {
            int32_t oo = o + 4 + OSE_BUNDLE_HEADER_LEN;
            int32_t ss = ose_readInt32(vm_s, oo);
            if(!strcmp(OSE_BUNDLE_ID, b + oo + 4))
            {
                if(s - OSE_BUNDLE_HEADER_LEN == ss + 4)
                {
                    ose_unpackDrop(vm_s);
                }
            }
        }
    }
    ose_pushString(vm_c, "/!/apply");
    ose_swap(vm_c);
}

static void oscript_execLambdaApplication(ose_bundle osevm)
{
    ose_bundle vm_i = OSEVM_INPUT(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_bundle vm_d = OSEVM_DUMP(osevm);

    if(ose_peekType(vm_s) == OSETT_BUNDLE)
    {
        // reverse the order
        // ose_popAllDropBundle(vm_s);
    }

    // int32_t toplevel = !ose_bundleHasAtLeastNElems(vm_c, 2);
    {
        // move anything remaining to do from control
        // back into input, since it will have do be done
        // *after* our finalization commands
        ose_drop(vm_c);
        ose_bundleAll(vm_c);
        ose_moveElem(vm_c, vm_i);
        ose_unpackDrop(vm_i);
        ose_pushString(vm_c, "/!/o/exec");
    }

    ose_pushString(vm_i, "/!/o/finalize/execlambdaapp");
    ose_copyBundle(vm_i, vm_d);
    ose_clear(vm_i);

    // copy env to dump
    ose_copyBundle(vm_e, vm_d);

    // put topmost stack element into input
    ose_moveElem(vm_s, vm_i);
    if(ose_peekType(vm_i) == OSETT_BUNDLE)
    {
    	ose_popAllDrop(vm_i);
    }

    {
        ose_pushMessage(vm_i,
                        "/!/o/finalize/elem",
                        strlen("/!/o/finalize/elem"),
                        0);
    }

    // store stack in dump
    ose_copyBundle(vm_s, vm_d);

    // prepare stack
    ose_clear(vm_s);
    // ose_pushBundle(vm_s);

    // move control to dump
    ose_drop(vm_c);             // drop our own exec command
    ose_copyBundle(vm_c, vm_d);
    ose_clear(vm_c);
}

static void oscript_exec(ose_bundle osevm)
{
    ose_bundle vm_i = OSEVM_INPUT(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_bundle vm_d = OSEVM_DUMP(osevm);

    if(ose_peekType(vm_s) == OSETT_BUNDLE)
    {
        // reverse the order
        // ose_popAllDropBundle(vm_s);
    }

    // int32_t toplevel = !ose_bundleHasAtLeastNElems(vm_c, 2);
    {
        // move anything remaining to do from control
        // back into input, since it will have do be done
        // *after* our finalization commands
        ose_drop(vm_c);
        ose_bundleAll(vm_c);
        ose_moveElem(vm_c, vm_i);
        ose_unpackDrop(vm_i);
        ose_pushString(vm_c, "/!/o/exec");
    }

    ose_pushString(vm_i, "/!/o/finalize/exec");
    ose_copyBundle(vm_i, vm_d);
    ose_clear(vm_i);

    // copy env to dump
    ose_copyBundle(vm_e, vm_d);

    // put topmost stack element into input
    ose_moveElem(vm_s, vm_i);
    if(ose_peekType(vm_i) == OSETT_BUNDLE)
    {
    	ose_popAllDrop(vm_i);
    }

    {
        ose_pushMessage(vm_i,
                        "/!/o/finalize/elem",
                        strlen("/!/o/finalize/elem"),
                        0);
    }

    // store stack in dump
    ose_copyBundle(vm_s, vm_d);

    // prepare stack
    ose_clear(vm_s);
    // ose_pushBundle(vm_s);

    // move control to dump
    ose_drop(vm_c);             // drop our own exec command
    ose_copyBundle(vm_c, vm_d);
    ose_clear(vm_c);
}

static void oscript_finalizeElem(ose_bundle osevm)
{
    ose_bundle vm_i = OSEVM_INPUT(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    if(ose_bundleHasAtLeastNElems(vm_s, 2))
    {
        ose_bundleAll(vm_e);
        // least efficient way possible to do this...
        int32_t n = ose_getBundleElemCount(vm_s);
        char *b = ose_getBundlePtr(vm_s);
        if(n == 2
           && !strcmp(ose_peekString(vm_s), OSE_ADDRESS_ANONVAL)
           && !strcmp(b + OSE_BUNDLE_HEADER_LEN + 4,
                      OSE_BUNDLE_ID))
        {
            ose_drop(vm_s);
            ose_moveElem(vm_s, vm_e);
        }
        else
        {
            for(int32_t i = 0; i < n; ++i)
            {
                ose_rollBottom(vm_s);
                if(ose_peekType(vm_s) == OSETT_BUNDLE)
                {
                    ose_elemToBlob(vm_s);
                    ose_setTypetag_impl(vm_s, OSETT_BUNDLE);
                }
            }
            ose_builtin_assignStackToEnv(osevm);
        }
        ose_swap(vm_e);
        ose_unpackDrop(vm_e);
    }
    else if(!ose_bundleIsEmpty(vm_s))
    {
        if(ose_peekType(vm_s) == OSETT_MESSAGE
           && ose_peekMessageArgType(vm_s) == OSETT_STRING)
        {
            ose_moveStringToAddress(vm_s);
            ose_bundleAll(vm_e);
            ose_moveElem(vm_s, vm_e);
            ose_swap(vm_e);
            ose_unpackDrop(vm_e);
        }
        else
        {
            ose_bundleAll(vm_e);
            ose_moveElem(vm_s, vm_e);
            ose_swap(vm_e);
            ose_unpackDrop(vm_e);
        }
    }
    
    if(!ose_bundleIsEmpty(vm_i))
    {
        ose_pushString(vm_i, "/!/o/finalize/elem");
        ose_swap(vm_i);
        if(ose_peekType(vm_i) == OSETT_MESSAGE)
        {
            ose_pushString(vm_i, OSE_ADDRESS_ANONVAL);
            ose_push(vm_i);
            ose_swapStringToAddress(vm_i);
        }
        else
        {
            ose_pushString(vm_i, OSE_ADDRESS_ANONVAL);
            ose_swap(vm_i);
        }
    }
}

static void oscript_finalizeExec(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    int32_t o = ose_getLastBundleElemOffset(vm_s);
    int32_t ss = ose_readInt32(vm_s, o);
    int32_t se = ose_readSize(vm_e);
    int32_t diff = ss - se;
    if(se > OSE_BUNDLE_HEADER_LEN)
    {
        if(diff > 0)
        {
            char *b = ose_getBundlePtr(vm_s);
            ose_writeInt32(vm_s, o, diff + OSE_BUNDLE_HEADER_LEN);
            memset(b + o + 4 + diff + OSE_BUNDLE_HEADER_LEN,
                   0, se - OSE_BUNDLE_HEADER_LEN);
            ose_decSize(vm_s, se - OSE_BUNDLE_HEADER_LEN);
        }
        else
        {
            ose_drop(vm_s);
            return;
        }
    }
    ose_popAllDropBundle(vm_s);
}

static void oscript_finalizeExecLambdaApplication(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_popAllDropBundle(vm_s);
    ose_pop(vm_s);
    ose_nip(vm_s);
    ose_pop(vm_s);
    ose_nip(vm_s);
}

void oscript_exec_load(ose_bundle vm_s)
{
    ose_pushMessage(vm_s, "/o/exec", strlen("/o/exec"),
                    1, OSETT_ALIGNEDPTR, oscript_exec);
    ose_pushMessage(vm_s, "/o/execlambdaapp",
                    strlen("/o/execlambdaapp"),
                    1, OSETT_ALIGNEDPTR,
                    oscript_execLambdaApplication);
    ose_pushMessage(vm_s, "/o/finalize/exec",
                    strlen("/o/finalize/exec"),
                    1, OSETT_ALIGNEDPTR, oscript_finalizeExec);
    ose_pushMessage(vm_s, "/o/finalize/execlambdaapp",
                    strlen("/o/finalize/execlambdaapp"),
                    1, OSETT_ALIGNEDPTR,
                    oscript_finalizeExecLambdaApplication);
    ose_pushMessage(vm_s, "/o/finalize/elem",
                    strlen("/o/finalize/elem"),
                    1, OSETT_ALIGNEDPTR, oscript_finalizeElem);
    ose_pushMessage(vm_s, "/o/apply",
                    strlen("/o/apply"),
                    1, OSETT_ALIGNEDPTR, oscript_apply);
}
