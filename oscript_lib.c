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

#define OSCRIPT_WRAP_BUILTIN_BINOP(name)        \
    static void oscript_ ##name (ose_bundle osevm) \
    {                                           \
        ose_bundle vm_s = OSEVM_STACK(osevm);   \
        ose_popAllDrop(vm_s);                   \
        ose_ ##name (vm_s);                      \
    }

#define OSCRIPT_WRAP_BUILTIN(name)              \
    static void oscript_ ##name (ose_bundle osevm) \
    {                                           \
        ose_bundle vm_s = OSEVM_STACK(osevm);   \
        ose_unpackDrop(vm_s);                   \
        ose_ ##name (vm_s);                      \
    }

OSCRIPT_WRAP_BUILTIN_BINOP(add);
OSCRIPT_WRAP_BUILTIN_BINOP(sub);
OSCRIPT_WRAP_BUILTIN_BINOP(mul);
OSCRIPT_WRAP_BUILTIN_BINOP(div);
OSCRIPT_WRAP_BUILTIN_BINOP(mod);
OSCRIPT_WRAP_BUILTIN_BINOP(pow);
OSCRIPT_WRAP_BUILTIN_BINOP(neg);
OSCRIPT_WRAP_BUILTIN_BINOP(eql);
OSCRIPT_WRAP_BUILTIN_BINOP(neq);
OSCRIPT_WRAP_BUILTIN_BINOP(lte);
OSCRIPT_WRAP_BUILTIN_BINOP(lt);
OSCRIPT_WRAP_BUILTIN_BINOP(and);
OSCRIPT_WRAP_BUILTIN_BINOP(or);

OSCRIPT_WRAP_BUILTIN(nth);
OSCRIPT_WRAP_BUILTIN(blobToElem);

static void oscript_defn(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_elemToBlob(vm_s);
    ose_setTypetag_impl(vm_s, OSCRIPT_TYPETAG_LAMBDA);
}

static void oscript_route(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    if(ose_peekType(vm_s) != OSETT_BUNDLE)
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_TYPE);
        return;
    }
    ose_unpackDrop(vm_s);
    if(!ose_bundleHasAtLeastNElems(vm_s, 2))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    if(ose_peekType(vm_s) != OSETT_MESSAGE)
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_TYPE);
        return;
    }
    ose_swap(vm_s);
    if(ose_peekType(vm_s) == OSETT_MESSAGE)
    {
        char tt = ose_peekMessageArgType(vm_s);
        switch(tt)
        {
        case OSETT_BUNDLE:
            ose_setTypetag_impl(vm_s, OSETT_BLOB);
            // fall through
        case OSETT_BLOB:
            ose_blobToElem(vm_s);
            break;
        default:
            ose_swap(vm_s);
            ose_errno_set(vm_s, OSE_ERR_ELEM_TYPE);
            return;
        }
    }
    ose_swap(vm_s);
    ose_routeWithDelegation(vm_s);
}

static void oscript_map(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    ose_unpackDrop(vm_s);
    ose_builtin_map(osevm);
}

static void oscript_if(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    ose_unpackDrop(vm_s);
    if(!ose_bundleHasAtLeastNElems(vm_s, 3))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    if(ose_peekMessageArgType(vm_s) != OSETT_INT32)
    {
    	ose_builtin_toInt32(osevm);
    }
    ose_pushInt32(vm_s, 0);
    ose_neq(vm_s);
    ose_roll(vm_s);
    ose_drop(vm_s);
    if(ose_peekType(vm_s) == OSETT_MESSAGE
       && ose_peekMessageArgType(vm_s) == OSETT_BLOB)
    {
        ose_blobToElem(vm_s);
    }
    if(ose_peekType(vm_s) == OSETT_BUNDLE
       && (ose_countItems(vm_s), ose_popInt32(vm_s)) == 1)
    {
        ose_unpackDrop(vm_s);
    }
    ose_pushString(vm_c, "/!/drop");
    ose_swap(vm_c);
    ose_pushString(vm_c, "/!/exec1");
    ose_swap(vm_c);
}

static void oscript_first(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_pop(vm_s);
    ose_nip(vm_s);
    ose_popAllDropBundle(vm_s);
    ose_pop(vm_s);
    ose_nip(vm_s);
}

static void oscript_last(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_pop(vm_s);
    ose_nip(vm_s);
    ose_pop(vm_s);
    ose_nip(vm_s);
}

static void oscript_rest(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_pop(vm_s);
    ose_nip(vm_s);
    ose_pushInt32(vm_s, 1);
    ose_split(vm_s);
    ose_nip(vm_s);
}

static void oscript_butlast(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_pop(vm_s);
    ose_nip(vm_s);
    ose_pop(vm_s);
    ose_drop(vm_s);
}

#define OSCRIPT_PUSHFN(name)                                    \
    {                                                           \
        ose_pushMessage(vm_s, "/o/"#name, strlen("/o/"#name),   \
                        1, OSETT_ALIGNEDPTR, oscript_##name);   \
    }

void oscript_lib_load(ose_bundle vm_s)
{
    OSCRIPT_PUSHFN(add);
    OSCRIPT_PUSHFN(sub);
    OSCRIPT_PUSHFN(mul);
    OSCRIPT_PUSHFN(div);
    OSCRIPT_PUSHFN(mod);
    OSCRIPT_PUSHFN(pow);
    OSCRIPT_PUSHFN(neg);
    OSCRIPT_PUSHFN(eql);
    OSCRIPT_PUSHFN(neq);
    OSCRIPT_PUSHFN(lte);
    OSCRIPT_PUSHFN(lt);
    OSCRIPT_PUSHFN(and);
    OSCRIPT_PUSHFN(or);

    OSCRIPT_PUSHFN(nth);
    OSCRIPT_PUSHFN(first);
    OSCRIPT_PUSHFN(last);
    OSCRIPT_PUSHFN(rest);
    OSCRIPT_PUSHFN(butlast);

    ose_pushMessage(vm_s, "/o/blob/toelem",
                    strlen("/o/blob/toelem"),
                    1, OSETT_ALIGNEDPTR, oscript_blobToElem);
    OSCRIPT_PUSHFN(route);
    OSCRIPT_PUSHFN(map);
    OSCRIPT_PUSHFN(if);
    OSCRIPT_PUSHFN(defn);
}
