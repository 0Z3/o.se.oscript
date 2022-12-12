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
    {
        ose_bundleAll(vm_s);
        ose_pop(vm_s);
        ose_swap(vm_s);
        ose_pop(vm_s);
        ose_rot(vm_s);
    }
    
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

    ose_rollBottom(vm_s);
    ose_bundleAll(vm_s);
    ose_pop(vm_s);
    ose_swap(vm_s);
    ose_pop(vm_s);
    ose_swap(vm_s);
    oscript_bindFnArgs(osevm);
    ose_swap(vm_s);
    ose_push(vm_s);
    ose_swap(vm_s);
    ose_unpackDrop(vm_s);
    ose_rollBottom(vm_s);

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

static void oscript_funcall(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundleAll(vm_s);
    ose_pop(vm_s);
    oscript_apply(osevm);
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

static void pushsemaphores(ose_bundle bundle)
{
#define OSCRIPT_SEM_BEG_MAGIC_INT32 0x23424547
#define OSCRIPT_SEM_END_MAGIC_INT32 0x23454E44
    ose_bundleAll(bundle);
    /* ose_pushBundle(bundle); */
    ose_pushMessage(bundle, "#BEG", strlen("#BEG"),
                    1, OSETT_INT32, OSCRIPT_SEM_BEG_MAGIC_INT32);
    ose_swap(bundle);
    ose_unpackDrop(bundle);
    /* ose_pushBundle(bundle); */
    ose_pushMessage(bundle, "#END", strlen("#END"),
                    1, OSETT_INT32, OSCRIPT_SEM_END_MAGIC_INT32);
}

static int is_beg_semaphore(ose_bundle bundle,
                            int32_t size,
                            int32_t offset)
{
    if(size == OSE_BUNDLE_HEADER_LEN
       && !strcmp(ose_getBundlePtr(bundle) + offset + 4, "#BEG"))
        /* can also check magic number */
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static int is_end_semaphore(ose_bundle bundle,
                            int32_t size,
                            int32_t offset)
{
    if(size == OSE_BUNDLE_HEADER_LEN
       && !strcmp(ose_getBundlePtr(bundle) + offset + 4, "#END"))
        /* can also check magic number */
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void semaphore_to_bundle(ose_bundle bundle,
                                int32_t size,
                                int32_t offset)
{
    memcpy(ose_getBundlePtr(bundle) + offset + 4,
           OSE_BUNDLE_ID OSE_TIMETAG_NULL,
           OSE_BUNDLE_HEADER_LEN);
}

static void oscript_exec(ose_bundle osevm)
{
    ose_bundle vm_i = OSEVM_INPUT(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_bundle vm_d = OSEVM_DUMP(osevm);

    {
    	int32_t o = ose_getLastBundleElemOffset(vm_s);
        if(!strcmp(ose_getBundlePtr(vm_s) + o + 4,
                   OSE_BUNDLE_ID)
           && ose_readInt32(vm_s, o) == OSE_BUNDLE_HEADER_LEN)
        {
            /* ose_bundleAll(vm_e); */
            /* ose_pushBundle(vm_e); */
            /* ose_swap(vm_e); */
            /* ose_unpackDrop(vm_e); */
            /* ose_drop(vm_s); */
            return;
        }
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

    // push semaphores onto the top and bottom of the env
    pushsemaphores(vm_e);
}

static void oscript_finalizeTopLevelExec(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    /* ose_bundleAll(vm_e); */
    ose_moveElem(vm_s, vm_e);
    ose_unpackDrop(vm_e);
    /* ose_rollBottom(vm_e); */
    /* ose_unpackDrop(vm_e); */
}

static void oscript_finalizeElem(ose_bundle osevm)
{
    ose_bundle vm_i = OSEVM_INPUT(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_e = OSEVM_ENV(osevm);
    ose_bundle bndlenv = vm_e;
    int32_t ns = ose_getBundleElemCount(vm_s);
    if(ns >= 2)
    {
        const char * const s = ose_peekString(vm_s);
        if(s[3] == '/')
        {
            const char buf[4] = {s[0], s[1], s[2], 0};
            int32_t o = ose_getContextMessageOffset(osevm, buf);
            if(o >= 0)
            {
                bndlenv = ose_enterBundleAtOffset(osevm, o);
            }
        }
        
        char *b = ose_getBundlePtr(vm_s);
        if(ns == 2
           && !strcmp(s, OSE_ADDRESS_ANONVAL)
           && !strcmp(b + OSE_BUNDLE_HEADER_LEN + 4,
                      OSE_BUNDLE_ID))
        {
            /* ose_bundleAll(bndlenv); */
            /* ose_drop(vm_s); */
            /* ose_moveElem(vm_s, bndlenv); */
            /* ose_swap(bndlenv); */
            /* ose_unpackDrop(bndlenv); */
            int32_t bes = ose_readSize(bndlenv);
            char *be = ose_getBundlePtr(bndlenv);
            int32_t o = OSE_BUNDLE_HEADER_LEN;
            int32_t s = ose_readInt32(vm_s, o);
            ose_incSize(bndlenv, s + 4);
            memmove(be + o + s + 4,
                    be + o,
                    bes);
            memcpy(be + o, b + o, s + 4);
            ose_clear(vm_s);
        }
        else
        {
            /* ose_bundleAll(bndlenv); */
            ose_builtin_assignStackToEnv_impl(osevm,
                                              bndlenv,
                                              0,
                                              1,
                                              OSETT_BUNDLE);
            /* ose_swap(bndlenv); */
            /* ose_unpackDrop(bndlenv); */
        }
        
    }
    else if(ns == 1)
    {
        if(ose_peekType(vm_s) == OSETT_MESSAGE
           && ose_peekMessageArgType(vm_s) == OSETT_STRING)
        {
            if(strlen(ose_peekString(vm_s)) == 0)
            {
                ose_drop(vm_s);
            }
            else
            {
                ose_moveStringToAddress(vm_s);
                ose_bundleAll(vm_e);
                ose_moveElem(vm_s, vm_e);
                ose_swap(vm_e);
                ose_unpackDrop(vm_e);
            }
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
        char *bi = ose_getBundlePtr(vm_i);
        int32_t o = ose_getLastBundleElemOffset(vm_i);
        int32_t s = ose_readInt32(vm_i, o);
        int32_t new_msg_size = OSE_ADDRESS_ANONVAL_SIZE + 4 + 20;
        ose_incSize(vm_i,
                    new_msg_size + 4);
        memmove(bi + o + 4 + new_msg_size,
                bi + o,
                s + 4);
        ose_writeInt32(vm_i, o, new_msg_size);
        memcpy(bi + o + 4,
               OSE_ADDRESS_ANONVAL ",s\0\0/!/o/finalize/elem\0\0" ,
               new_msg_size);
        o += new_msg_size + 4;
        /* ose_pushString(vm_i, "/!/o/finalize/elem"); */
        /* ose_swap(vm_i); */
        if(ose_peekType(vm_i) == OSETT_MESSAGE)
        {
            /* { */
            /*     char buf[65536]; */
            /*     memset(buf, 0, 65536); */
            /*     ose_pprintBundle(vm_i, buf, 65536); */
            /*     fprintf(stderr, ">>>>>\n\r%s\n\r*****\n\r", */
            /*             buf); */
            /* } */
            /* ose_pushString(vm_i, OSE_ADDRESS_ANONVAL); */
            /* ose_push(vm_i); */
            /* ose_swapStringToAddress(vm_i); */
            int32_t ao = o + 4;
            int32_t as = ose_pstrlen(bi + ao);
            int32_t tto = ao + as;
            int32_t ntt = strlen(bi + tto);
            int32_t plo = tto + ose_pnbytes(ntt);
            if(ose_pnbytes(ntt + 1) != (plo - tto))
            {
                ose_incSize(vm_i, 4);
                memmove(bi + plo + 4,
                        bi + plo,
                        s - (plo - (o + 4)));
                bi[plo] = 0;
                bi[plo + 1] = 0;
                bi[plo + 2] = 0;
                bi[plo + 3] = 0;
                plo += 4;
                s += 4;
                ose_writeInt32(vm_i, o, s);
            }
            bi[tto + ntt] = OSETT_STRING;
            if(as == OSE_ADDRESS_ANONVAL_SIZE)
            {
                /* printf("here\n"); */
                ose_incSize(vm_i, OSE_ADDRESS_ANONVAL_SIZE);
                s += 4;
                ose_writeInt32(vm_i, o, s);
                memcpy(bi + o + s,
                       bi + o + 4,
                       OSE_ADDRESS_ANONVAL_SIZE);
                memcpy(bi + o + 4,
                       OSE_ADDRESS_ANONVAL,
                       OSE_ADDRESS_ANONVAL_SIZE);
            }
            else
            {
                /* printf("there\n"); */
                ose_incSize(vm_i, as);
                /* copy address to end of message */
                memcpy(bi + o + s + 4,
                       bi + o + 4,
                       as);
                /* shift everything left */
                memmove(bi + o + 4 + OSE_ADDRESS_ANONVAL_SIZE,
                        bi + tto,
                        (s - (tto - (o + 4))) + ao);
                /* copy anon address to address */
                memcpy(bi + ao,
                       OSE_ADDRESS_ANONVAL,
                       OSE_ADDRESS_ANONVAL_SIZE);
                /* memset extra bytes at end */
                memset(bi + o + s + 4 + (as - OSE_ADDRESS_ANONVAL_SIZE),
                       0,
                       as - OSE_ADDRESS_ANONVAL_SIZE);
                s += OSE_ADDRESS_ANONVAL_SIZE;
                ose_writeInt32(vm_i, o, s);
                ose_decSize(vm_i,
                            (as - OSE_ADDRESS_ANONVAL_SIZE));
            }
            /* { */
            /*     char buf[65536]; */
            /*     memset(buf, 0, 65536); */
            /*     ose_pprintBundle(vm_i, buf, 65536); */
            /*     fprintf(stderr, "%s\n\r<<<<<\n\r", */
            /*             buf); */
            /* } */
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
    /* 
       The stack has a bundle that looks like this:
		#bundle
			<our stuff>
			...
			#bundle <--semaphore
			<not our stuff>
			#bundle <--semaphore
			<more of our stuff>
			...
    */
    /* int32_t o = OSE_BUNDLE_HEADER_LEN; */
    const int32_t lbeo = ose_getLastBundleElemOffset(vm_s);
    int32_t o = lbeo;
    int32_t s = ose_readInt32(vm_s, o);
    int32_t oo = o + 4 + OSE_BUNDLE_HEADER_LEN;
    int32_t ss;
    /* 
       seek forward in the bundle to the first semaphore, and
       rewrite the bundle so that it only contains our stuff 
    */
    while(oo < o + s)
    {
        ss = ose_readInt32(vm_s, oo);
        /* if(ss == OSE_BUNDLE_HEADER_LEN */
        /*    && !strcmp(b + oo + 4, */
        /*               OSE_BUNDLE_ID)) */
        if(is_beg_semaphore(vm_s, ss, oo))
        {
            semaphore_to_bundle(vm_s, ss, oo);
            ose_writeInt32(vm_s, o, oo - o - 4);
            ose_writeInt32(vm_s, oo, s - (oo - o));
            break;
        }
        oo += ss + 4;
    }
    /*
       continue seeking forward in the bundle to find the final
       semaphore 
    */
    o = oo;
    s = ose_readInt32(vm_s, o);
    oo += OSE_BUNDLE_HEADER_LEN + 4;
    int32_t lso;
    while(oo < o + s)
    {
        ss = ose_readInt32(vm_s, oo);
        /* if(ss == OSE_BUNDLE_HEADER_LEN */
        /*    && !strcmp(b + oo + 4, */
        /*               OSE_BUNDLE_ID)) */
        if(is_end_semaphore(vm_s, ss, oo))
        {
            semaphore_to_bundle(vm_s, ss, oo);
            lso = oo;
        }
        oo += ss + 4;
    }
    /*
       rewrite the bundle so that our stuff is in its own at 
       the end
    */
    ose_writeInt32(vm_s, o, lso - o - 4);
    ose_writeInt32(vm_s, lso, (oo - lso) - 4);
    /* 
       discard the stuff that's not ours, and combine the two
       bundles
    */
    ose_nip(vm_s);
    ose_swap(vm_s);
    ose_popAllDropBundle(vm_s);
    ose_swap(vm_s);
    ose_unpackDrop(vm_s);
    ose_writeInt32(vm_s, lbeo,
                   (ose_readSize(vm_s)
                    - (lbeo + 4)));
}

static void oscript_finalizeExecLambdaApplication(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char *b = ose_getBundlePtr(vm_s);
    int32_t oldsize = ose_readSize(vm_s);
    int32_t o = ose_getLastBundleElemOffset(vm_s);
    int32_t s = ose_readInt32(vm_s, o);
    int32_t oo = o + OSE_BUNDLE_HEADER_LEN + 4;
    int32_t ss = ose_readInt32(vm_s, oo);
    int32_t ooo = oo + OSE_BUNDLE_HEADER_LEN + 4;
    int32_t sss;
    while(1)
    {
        sss = ose_readInt32(vm_s, ooo);
        if(ooo + sss + 4 >= oo + ss + 4)
        {
            break;
        }
        ooo = ooo + sss + 4;
    }
    ose_incSize(vm_s, sss + 4);
    memcpy(b + oldsize, b + ooo, sss + 4);
    memset(b + o, 0, s + 4);
    memcpy(b + o, b + oldsize, sss + 4);
    memset(b + oldsize, 0, sss + 4);
    ose_decSize(vm_s, s + 4);
    /* ose_popAllDropBundle(vm_s); */
    /* ose_pop(vm_s); */
    /* ose_nip(vm_s); */
    /* ose_pop(vm_s); */
    /* if(ose_bundleHasAtLeastNElems(vm_s, 2)) */
    /* { */
    /* 	ose_nip(vm_s); */
    /* } */
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
    ose_pushMessage(vm_s, "/o/finalize/toplevelexec",
                    strlen("/o/finalize/toplevelexec"),
                    1,
                    OSETT_ALIGNEDPTR,
                    oscript_finalizeTopLevelExec);
    ose_pushMessage(vm_s, "/o/apply",
                    strlen("/o/apply"),
                    1, OSETT_ALIGNEDPTR, oscript_apply);
    ose_pushMessage(vm_s, "/o/funcall",
                    strlen("/o/funcall"),
                    1, OSETT_ALIGNEDPTR, oscript_funcall);
}
