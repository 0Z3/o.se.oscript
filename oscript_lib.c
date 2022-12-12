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

#define OSCRIPT_WRAP_BUILTIN_BINOP(name)            \
    static void oscript_ ##name (ose_bundle osevm)  \
    {                                               \
        ose_bundle vm_s = OSEVM_STACK(osevm);       \
        enum ose_errno e = OSE_ERR_NONE;            \
        ose_popAllDrop(vm_s);                       \
        ose_ ##name (vm_s);                         \
        if((e = ose_errno_get(vm_s)))               \
        {                                           \
            ose_errno_set(osevm, e);                \
            ose_errno_set(vm_s, OSE_ERR_NONE);      \
        }                                           \
    }

#define OSCRIPT_WRAP_BUILTIN(name)                  \
    static void oscript_ ##name (ose_bundle osevm)  \
    {                                               \
        ose_bundle vm_s = OSEVM_STACK(osevm);       \
        enum ose_errno e = OSE_ERR_NONE;            \
        ose_unpackDrop(vm_s);                       \
        ose_ ##name (vm_s);                         \
        if((e = ose_errno_get(vm_s)))               \
        {                                           \
            ose_errno_set(osevm, e);                \
            ose_errno_set(vm_s, OSE_ERR_NONE);      \
        }                                           \
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

/* 
{
	: [<nelems>, <current count>], # count message
	{
		: [<elem typetag>, <elem offset>, <offset1>, <offset2>],
		...
	},
	{
		...
	}
}
 */

/* 
   CM: Count Message 
   offsets are relative to the offset of the map bundle
*/
#define MAP_CM_N 3
#define MAP_CM_ADDR_SIZE OSE_ADDRESS_ANONVAL_SIZE
#define MAP_CM_TT_SIZE 8
#define MAP_CM_PL_SIZE (MAP_CM_N * 4)

#define MAP_CM_OFFSET (4 + OSE_BUNDLE_HEADER_LEN)
#define MAP_CM_SIZE (MAP_CM_ADDR_SIZE           \
                     + MAP_CM_TT_SIZE           \
                     + MAP_CM_PL_SIZE)
#define MAP_CM_ADDR_OFFSET (MAP_CM_OFFSET + 4)
#define MAP_CM_TT_OFFSET (MAP_CM_ADDR_OFFSET + MAP_CM_ADDR_SIZE)
#define MAP_CM_PL_OFFSET (MAP_CM_TT_OFFSET + MAP_CM_TT_SIZE)
#define MAP_CM_PL0_OFFSET MAP_CM_PL_OFFSET
#define MAP_CM_PL1_OFFSET (MAP_CM_PL0_OFFSET + 4)
#define MAP_CM_PL2_OFFSET (MAP_CM_PL1_OFFSET + 4)

#define MAP_CM_ARGBNDL_OFFSET MAP_CM_PL0_OFFSET
#define MAP_CM_NELEMS_OFFSET MAP_CM_PL1_OFFSET
#define MAP_CM_COUNT_OFFSET MAP_CM_PL2_OFFSET

#define MAP_GET_ARGBNDLOFFSET(b, o)                     \
    ose_readInt32(b, o + MAP_CM_ARGBNDL_OFFSET)
#define MAP_GET_NELEMS(b, o)                    \
    ose_readInt32(b, o + MAP_CM_NELEMS_OFFSET)
#define MAP_GET_COUNT(b, o)                     \
    ose_readInt32(b, o + MAP_CM_COUNT_OFFSET)

#define MAP_SET_COUNT(b, o, v)                    \
    ose_writeInt32(b, o + MAP_CM_COUNT_OFFSET, v)

/* 
   OM: Offset Message
*/
#define MAP_ON_N 4
#define MAP_OM_ADDR_SIZE 4
#define MAP_OM_TT_SIZE 8
#define MAP_OM_PL_SIZE (MAP_OM_ADDR_SIZE * 16)

#define MAP_OM0_OFFSET (MAP_CM_OFFSET                   \
                        + MAP_CM_SIZE + 4               \
                        + OSE_BUNDLE_HEADER_LEN + 4)
#define MAP_OM_SIZE                                 \
    (4                        /* addr */            \
     + 8                        /* typetags */      \
     + 4                        /* elem type */     \
     + 4                        /* elem offset */   \
     + 4                        /* offset 1 */      \
     + 4)                        /* offset 2 */
#define MAP_OM0_ADDR_OFFSET (MAP_OM0_OFFSET + 4)
#define MAP_OM0_TT_OFFSET (MAP_OM0_ADDR_OFFSET + MAP_OM_ADDR_SIZE)
#define MAP_OM0_PL_OFFSET (MAP_OM0_TT_OFFSET + MAP_OM_TT_SIZE)
#define MAP_OM0_PL0_OFFSET MAP_OM0_PL_OFFSET
#define MAP_OM0_PL1_OFFSET (MAP_OM0_PL0_OFFSET + 4)
#define MAP_OM0_PL2_OFFSET (MAP_OM0_PL1_OFFSET + 4)
#define MAP_OM0_PL3_OFFSET (MAP_OM0_PL2_OFFSET + 4)

#define MAP_OM0_ELEMTYPE_OFFSET MAP_OM0_PL0_OFFSET
#define MAP_OM0_ELEMOFFSET_OFFSET MAP_OM0_PL1_OFFSET
#define MAP_OM0_OFFSET1_OFFSET MAP_OM0_PL2_OFFSET
#define MAP_OM0_OFFSET2_OFFSET MAP_OM0_PL3_OFFSET

#define MAP_OM0_BNDL_ELEM_OFFSET MAP_OM0_OFFSET1_OFFSET
#define MAP_OM0_MSG_TYPETAG_OFFSET MAP_OM0_OFFSET1_OFFSET
#define MAP_OM0_MSG_PAYLOAD_OFFSET MAP_OM0_OFFSET2_OFFSET

#define MAP_OM_REL2ABS_OFFSET(o, oo, n)         \
    (o + oo + (n * (MAP_OM_SIZE + 4)))

#define MAP_GET_ELEMTYPE(b, o, n)               \
    ose_readInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_ELEMTYPE_OFFSET, \
                                        n))
#define MAP_GET_ELEMOFFSET(b, o, n)             \
    ose_readInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_ELEMOFFSET_OFFSET, \
                                        n))
#define MAP_GET_OFFSET1(b, o, n)                \
    ose_readInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_OFFSET1_OFFSET, \
                                        n))
#define MAP_GET_OFFSET2(b, o, n)                \
    ose_readInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_OFFSET2_OFFSET, \
                                        n))
#define MAP_GET_BNDL_ELEM_OFFSET(b, o, n)       \
    MAP_GET_OFFSET1(b, o, n)
#define MAP_GET_MSG_TYPETAG_OFFSET(b, o, n)     \
    MAP_GET_OFFSET1(b, o, n)
#define MAP_GET_MSG_PAYLOAD_OFFSET(b, o, n)     \
    MAP_GET_OFFSET2(b, o, n)

#define MAP_SET_OFFSET1(b, o, n, v)              \
    ose_writeInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_OFFSET1_OFFSET, \
                                        n),\
                   v)
#define MAP_SET_OFFSET2(b, o, n, v)              \
    ose_writeInt32(b, \
                  MAP_OM_REL2ABS_OFFSET(o, \
                                        MAP_OM0_OFFSET2_OFFSET, \
                                        n),\
                   v)
#define MAP_SET_BNDL_ELEM_OFFSET(b, o, n, v)\
    MAP_SET_OFFSET1(b, o, n, v)
#define MAP_SET_MSG_TYPETAG_OFFSET(b, o, n, v)\
    MAP_SET_OFFSET1(b, o, n, v)
#define MAP_SET_MSG_PAYLOAD_OFFSET(b, o, n, v)\
    MAP_SET_OFFSET2(b, o, n, v)

static void oscript_mapRest_impl(ose_bundle osevm)
{
    /* 
       stack should be:
       fn (blob)
       bundle

       no need to check args, since any issues should have
       already been caught by the caller
    */
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char *bs = ose_getBundlePtr(vm_s);
    const int32_t o = ose_getLastBundleElemOffset(vm_s);

    int32_t argbndloffset = MAP_GET_ARGBNDLOFFSET(vm_s, o);
    int32_t nelems = MAP_GET_NELEMS(vm_s, o);
    int32_t count = MAP_GET_COUNT(vm_s, o);

    int32_t bundleoffset = ose_readSize(vm_s);
    OSE_START_BUNDLE(vm_s);
    for(int j = 0; j < nelems; ++j)
    {
    	char elemtype = (char)MAP_GET_ELEMTYPE(vm_s, o, j);
        int32_t elemoffset = o + argbndloffset
            + MAP_GET_ELEMOFFSET(vm_s, o, j);
        if(elemtype == OSETT_BUNDLE)
        {
            int32_t bsize = ose_readInt32(vm_s, elemoffset);
            int32_t beoffset = o + argbndloffset +
                MAP_GET_BNDL_ELEM_OFFSET(vm_s, o, j);
            if(beoffset >= elemoffset + bsize + 4)
            {
                int32_t ssize = ose_readSize(vm_s);
                int32_t diff = ssize - bundleoffset;
                if(diff > 0)
                {
                    memset(bs + bundleoffset, 0, diff);
                    ose_decSize(vm_s, diff);
                }
                ose_2drop(vm_s);
                return;
            }
            int32_t besize = ose_readInt32(vm_s, beoffset);
            {
                int32_t oldsize = ose_readSize(vm_s);
                ose_incSize(vm_s, besize + 4);
                memset(bs + oldsize, 0, besize + 4);
                memcpy(bs + oldsize, bs + beoffset, besize + 4);
                /* ose_writeInt32(vm_s, oldsize, newmsgsize); */
                /* bs[oldsize + 8] = OSETT_ID; */
                /* bs[oldsize + 9] = tt; */
                /* memcpy(bs + oldsize + 12, */
                /*        bs + payloadoffset, */
                /*        pls); */
            }
            {
                MAP_SET_BNDL_ELEM_OFFSET(vm_s, o, j,
                                         (beoffset
                                          - (o + argbndloffset))
                                         + (besize + 4));
            }
        }
        else
        {
            int32_t typetagoffset = o + argbndloffset + 
                MAP_GET_MSG_TYPETAG_OFFSET(vm_s, o, j);
            int32_t payloadoffset = o + argbndloffset + 
                MAP_GET_MSG_PAYLOAD_OFFSET(vm_s, o, j);
            char tt = ose_readByte(vm_s, typetagoffset);
            if(tt == 0)
            {
                int32_t ssize = ose_readSize(vm_s);
                int32_t diff = ssize - bundleoffset;
                if(diff > 0)
                {
                    memset(bs + bundleoffset, 0, diff);
                    ose_decSize(vm_s, diff);
                }
                ose_2drop(vm_s);
                return;
            }
            int32_t pls =
                ose_getPayloadItemSize(vm_s,
                                       tt,
                                       elemoffset,
                                       payloadoffset);
            {
                int32_t oldsize = ose_readSize(vm_s);
                int32_t newmsgsize = 8 + pls;
                ose_incSize(vm_s, newmsgsize + 4);
                memset(bs + oldsize, 0, newmsgsize + 4);
                ose_writeInt32(vm_s, oldsize, newmsgsize);
                bs[oldsize + 8] = OSETT_ID;
                bs[oldsize + 9] = tt;
                memcpy(bs + oldsize + 12,
                       bs + payloadoffset,
                       pls);
            }
            {
                MAP_SET_MSG_TYPETAG_OFFSET(vm_s, o, j,
                                           (typetagoffset
                                            - (o + argbndloffset))
                                           + 1);
                MAP_SET_MSG_PAYLOAD_OFFSET(vm_s, o, j,
                                           (payloadoffset
                                            - (o + argbndloffset))
                                           + pls);
            }
        }
    }
    OSE_END_BUNDLE(vm_s);
    MAP_SET_COUNT(vm_s, o, count + 1);
    {
        /* ose_notrot(vm_s); */
        /* ose_drop(vm_c);     /\* our command *\/ */
        /* ose_pushString(vm_c, "/!/o/maprest"); */
        /* ose_elemToBlob(vm_s); */
        /* ose_moveElem(vm_s, vm_c); */
        /* ose_moveElem(vm_s, vm_c); */
        /* ose_dup(vm_c); */
        /* ose_pushString(vm_c, "/!/o/apply"); */
        /* ose_swap(vm_c); */
        /* ose_pushString(vm_c, ""); */

        /* 
           There are three elements on the
           stack, and we want to move two of
           them to control, and leave the third
           on the stack:

           STACK:		CONTROL:
           E_nm2		...
           E_nm1		<our command>
           E_n

		   STACK:		CONTROL:
           E_n			...
						[s:/!/o/maprest]
						[b:E_nm1]
						E_nm2
						[s:/!/o/apply]
						E_nm2
						[s:] <-- will be dropped
        */
        char *bc = ose_getBundlePtr(vm_c);
        int32_t co = ose_getLastBundleElemOffset(vm_c);
        int32_t cs = ose_readInt32(vm_c, co);
        int32_t s = ose_readSize(vm_s);
        int32_t onm2 = OSE_BUNDLE_HEADER_LEN;
        int32_t snm2 = ose_readInt32(vm_s, onm2);
        int32_t onm1 = onm2 + snm2 + 4;
        int32_t snm1 = ose_readInt32(vm_s, onm1);
        int32_t on = onm1 + snm1 + 4;
        int32_t sn = ose_readInt32(vm_s, on);
        while(on + sn + 4 < s)
        {
            onm2 = onm1;
            snm2 = ose_readInt32(vm_s, onm2);
            onm1 = on;
            snm1 = ose_readInt32(vm_s, onm1);
            on += sn + 4;
            sn = ose_readInt32(vm_s, on);
        }
        int32_t addrsize = OSE_ADDRESS_ANONVAL_SIZE;
        int32_t typetagsize = 4;
        int32_t msg_maprest_size =
            addrsize + typetagsize + 16;
        int32_t msg_enm1_size =
            addrsize + typetagsize + snm1 + 4;
        int32_t msg_apply_size = addrsize + typetagsize + 12;
        int32_t msg_empty_size = addrsize + typetagsize;
        int32_t amt =
            4 + msg_maprest_size +
            4 + msg_enm1_size +
            snm2 + 4 +
            4 + msg_apply_size +
            snm2 + 4 +
            4 + msg_empty_size;

        ose_incSize(vm_c, amt - (cs + 4));

        int32_t o = co;
        {
            ose_writeInt32(vm_c, o, msg_maprest_size);
            o += 4;
            memcpy(bc + o,
                   OSE_ADDRESS_ANONVAL,
                   OSE_ADDRESS_ANONVAL_SIZE);
            o += OSE_ADDRESS_ANONVAL_SIZE;
            bc[o++] = OSETT_ID;
            bc[o++] = OSETT_STRING;
            bc[o++] = 0;
            bc[o++] = 0;
            memcpy(bc + o, "/!/o/maprest\0\0\0\0", 16);
            o += 16;
        }
        {
            ose_writeInt32(vm_c, o, msg_enm1_size);
            o += 4;
            memcpy(bc + o,
                   OSE_ADDRESS_ANONVAL,
                   OSE_ADDRESS_ANONVAL_SIZE);
            o += OSE_ADDRESS_ANONVAL_SIZE;
            bc[o++] = OSETT_ID;
            bc[o++] = OSETT_BLOB;
            bc[o++] = 0;
            bc[o++] = 0;
            memcpy(bc + o, bs + onm1, snm1 + 4);
            o += snm1 + 4;
        }
        {
            memcpy(bc + o, bs + onm2, snm2 + 4);
            o += snm2 + 4;
        }
        {
            ose_writeInt32(vm_c, o, msg_apply_size);
            o += 4;
            memcpy(bc + o,
                   OSE_ADDRESS_ANONVAL,
                   OSE_ADDRESS_ANONVAL_SIZE);
            o += OSE_ADDRESS_ANONVAL_SIZE;
            bc[o++] = OSETT_ID;
            bc[o++] = OSETT_STRING;
            bc[o++] = 0;
            bc[o++] = 0;
            memcpy(bc + o, "/!/o/apply\0\0", 12);
            o += 12;
        }
        {
            memcpy(bc + o, bs + onm2, snm2 + 4);
            o += snm2 + 4;
        }
        {
            ose_writeInt32(vm_c, o, msg_empty_size);
            o += 4;
            memcpy(bc + o,
                   OSE_ADDRESS_ANONVAL,
                   OSE_ADDRESS_ANONVAL_SIZE);
            o += OSE_ADDRESS_ANONVAL_SIZE;
            bc[o++] = OSETT_ID;
            bc[o++] = 0;
            bc[o++] = 0;
            bc[o++] = 0;
        }

        {
            memset(bs + onm2, 0, snm2 + snm1 + 8);
            memmove(bs + onm2, bs + on, sn + 4);
            memset(bs + on, 0, sn + 4);
            ose_decSize(vm_s, snm2 + snm1 + 8);
        }
    }
}

static void oscript_mapRest(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 2))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    ose_blobToElem(vm_s);
    oscript_mapRest_impl(osevm);
}

static void oscript_map(ose_bundle osevm)
{
    /* 
       /o/map maps a function (2nd arg) onto each item of
       each elem of a bundle (1st arg)

       first arg is a bundle or a message.
       if it's a message, iterate over the items:

       if it's a bundle, iterate over the items in each elem:

       ({[1, 2, 3], [4, 5, 6]}, /o/add) /o/map
       => (1, 4) /o/add
       => (2, 5) /o/add
       => (3, 6) /o/add

       ({{[1, 2, 3], [4, 5, 6]}}, /o/println) /o/map
       => ([1, 2, 3]) /o/println
       => ([4, 5, 6]) /o/println

       if it's a message, it's equivalent to that message being
       wrapped in a bundle--iterate over the items in the single
       elem:

       ([1, 2, 3], /o/println) /o/map
       == ({[1, 2, 3]}, /o/println) /o/map
       => (1) /o/println
       => (2) /o/println
       => (3) /o/println
    */
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    ose_unpackDrop(vm_s);
    if(!ose_bundleHasAtLeastNElems(vm_s, 2))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    ose_swap(vm_s);
    if(ose_peekType(vm_s) == OSETT_MESSAGE)
    {
        if(ose_peekMessageArgType(vm_s) == OSETT_BLOB)
        {
            ose_blobToElem(vm_s);
        }
        else
        {
            ose_swap(vm_s);
            ose_errno_set(osevm, OSE_ERR_ITEM_TYPE);
            return;
        }
    }

    if(ose_peekType(vm_s) != OSETT_BUNDLE)
    {
        ose_swap(vm_s);
        ose_errno_set(osevm, OSE_ERR_ELEM_TYPE);
        return;
    }

    const char * const b = ose_getBundlePtr(vm_s);
    int32_t o = ose_getLastBundleElemOffset(vm_s);
    int32_t s = ose_readInt32(vm_s, o);
    int32_t oo = o + 4 + OSE_BUNDLE_HEADER_LEN;
    int32_t ss = ose_readInt32(vm_s, oo);;
    int32_t n = 0;
    OSE_START_BUNDLE(vm_s);
    while(oo < (o + s + 4))
    {
        /* 
           typetag (bundle or message)
		   elem offset
           first offset (message: typetag, bundle: elem)
           second offset (message: payload, bundle: not used)
        */
        if(!strcmp(b + oo + 4, OSE_BUNDLE_ID))
        {
            ose_pushMessage(vm_s,
                            OSE_ADDRESS_ANONVAL,
                            OSE_ADDRESS_ANONVAL_LEN,
                            4,
                            OSETT_INT32, OSETT_BUNDLE, /* tt */
                            OSETT_INT32, oo - o, /* elem */
                            OSETT_INT32,     /* o1 */
                            (oo - o) + 4 + OSE_BUNDLE_HEADER_LEN,
                            OSETT_INT32, 0); /* o2 */
        }
        else
        {
            int32_t ao = oo + 4;
            int32_t tto = ao + ose_pstrlen(b + ao);
            int32_t plo = tto + ose_pstrlen(b + tto);
            ose_pushMessage(vm_s,
                            OSE_ADDRESS_ANONVAL,
                            OSE_ADDRESS_ANONVAL_LEN,
                            4,
                            OSETT_INT32, OSETT_MESSAGE, /* tt */
                            OSETT_INT32, oo - o, /* elem */
                            OSETT_INT32, (tto - o) + 1, /* o1 */
                            OSETT_INT32, (plo - o));    /* o2 */
        }
        ++n;
        oo += (ss + 4);
        ss = ose_readInt32(vm_s, oo);
    }
    OSE_END_BUNDLE(vm_s);
    /* 
	   offset of arg bundle inside the map bundle
       number of elems
       current count
    */
    ose_pushBundle(vm_s);
    ose_pushMessage(vm_s,
                    OSE_ADDRESS_ANONVAL,
                    OSE_ADDRESS_ANONVAL_LEN,
                    3,
                    OSETT_INT32, (
                        4 + OSE_BUNDLE_HEADER_LEN
                        + 4 + MAP_CM_SIZE
                        + 4 + OSE_BUNDLE_HEADER_LEN
                        + (n * (4 + MAP_OM_SIZE))
                        ),
                    OSETT_INT32, n,
                    OSETT_INT32, 0);
    ose_push(vm_s);
    ose_swap(vm_s);
    ose_push(vm_s);
    ose_swap(vm_s);
    ose_push(vm_s);
    /* 
       {
			[n, count],
			{
				[tt, o1, o2],
				...
			},
			{
				...
			}
       }
    */
    oscript_mapRest_impl(osevm);
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
    ose_pushString(vm_c, "/!/o/exec");
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

struct oscript_sort_item
{
    ose_bundle bundle;
    int32_t typetag, offset, size, index;
};

static int sort_numToDouble(ose_constbundle bundle,
                            const char tt,
                            const int32_t offset)
{
    if(ose_isIntegerType(tt))
    {
#ifdef OSE_PROVIDE_TYPE_INT64
        if(tt == OSETT_INT64 || tt == OSETT_UINT64)
        {
        }
        else
#endif
        {
#ifdef OSE_PROVIDE_TYPE_UINT32
            if(tt == OSETT_UINT32)
            {

            }
            else
#endif
            {
                return (double)ose_readInt32(bundle, offset);
            }
        }
    }
    else if(ose_isFloatType(tt))
    {
        switch(tt)
        {
        case OSETT_FLOAT:
            return (double)ose_readFloat(bundle, offset);
#ifdef OSE_PROVIDE_TYPE_DOUBLE
        case OSETT_DOUBLE:
            return ose_readDouble(bundle, offset);
#endif
        }
    }
    return 0;
}

static int sort_cb(const void *v1, const void *v2)
{
    const struct oscript_sort_item *i1 = (const struct oscript_sort_item *)v1;
    const struct oscript_sort_item *i2 = (const struct oscript_sort_item *)v2;
    char tt1 = i1->typetag, tt2 = i2->typetag;
    if(tt1 == OSETT_STRING && tt2 == OSETT_STRING)
    {
        return strcmp(ose_readString(i1->bundle, i1->offset),
                      ose_readString(i2->bundle, i2->offset));
    }
    else if(tt1 == OSETT_STRING)
    {
        return 1;
    }
    else if(tt2 == OSETT_STRING)
    {
        return -1;
    }
    else
    {
        double d1 = sort_numToDouble(i1->bundle, tt1, i1->offset);
        double d2 = sort_numToDouble(i2->bundle, tt2, i2->offset);
        if(d1 < d2)
        {
            return -1;
        }
        else if(d2 < d1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

static void oscript_sort_impl(ose_bundle osevm,
                              int32_t vals,
                              int32_t idxs)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    char *b = ose_getBundlePtr(vm_s);
    if(ose_bundleIsEmpty(vm_s))
    {
        ose_errno_set(osevm, OSE_ERR_ELEM_COUNT);
        return;
    }
    if(ose_peekType(vm_s) != OSETT_BUNDLE)
    {
        ose_errno_set(osevm, OSE_ERR_ELEM_TYPE);
        return;
    }
    ose_unpackDrop(vm_s);
    if(ose_peekType(vm_s) != OSETT_MESSAGE)
    {
        ose_errno_set(osevm, OSE_ERR_ELEM_TYPE);
        return;
    }
    const int32_t o = ose_getLastBundleElemOffset(vm_s);
    const int32_t s = ose_readInt32(vm_s, o);
    const int32_t ao = o + 4;
    const int32_t asize = ose_pstrlen(b + ao);
    const int32_t tto = ao + asize;
    const int32_t ntt = strlen(b + tto);
    const int32_t plo = tto + ose_pnbytes(ntt);
    int32_t tto_ = tto + 1;
    int32_t plo_ = plo;
    struct oscript_sort_item items[ntt - 1];
    for(int32_t i = 0; i < ntt - 1; ++i)
    {
        items[i].bundle = vm_s;
        char tt = ose_readByte(vm_s, tto_);
        items[i].typetag = tt;
        items[i].offset = plo_;
        items[i].index = i;
        int32_t size = ose_getPayloadItemSize(vm_s, tt, o, plo_);
        items[i].size = size;
        plo_ += size;
        ++tto_;
    }
    qsort((void *)items,
          ntt - 1,
          sizeof(struct oscript_sort_item),
          sort_cb);

    if(vals)
    {
        ose_incSize(vm_s, s + 4);
        const int32_t no = o + s + 4;
        memset(b + no, 0, s + 4);
        ose_writeInt32(vm_s, no, s);
        const int32_t nao = no + 4;
        memcpy(b + nao, b + ao, asize);
        int32_t ntto_ = nao + asize;
        int32_t nplo_ = ntto_ + (plo - tto);
        b[ntto_++] = OSETT_ID;
        for(int32_t i = 0; i < ntt - 1; ++i)
        {
            b[ntto_++] = items[i].typetag;
            memcpy(b + nplo_, b + items[i].offset, items[i].size);
            nplo_ += items[i].size;
        }
        /* memcpy(b + o, b + no, s + 4); */
        /* memset(b + no, 0, s + 4); */
        /* ose_decSize(vm_s, s + 4); */
    }
    if(idxs)
    {
        
        const int32_t no = ose_readSize(vm_s);
        const int32_t ns = asize
            + ose_pnbytes(ntt)
            + (4 * (ntt - 1));
        ose_incSize(vm_s, ns + 4);
        memset(b + no, 0, ns + 4);
        ose_writeInt32(vm_s, no, ns);
        const int32_t nao = no + 4;
        memcpy(b + nao, b + ao, asize);
        int32_t ntto_ = nao + asize;
        int32_t nplo_ = ntto_ + (plo - tto);
        b[ntto_++] = OSETT_ID;
        for(int32_t i = 0; i < ntt - 1; ++i)
        {
            b[ntto_++] = OSETT_INT32;
            ose_writeInt32(vm_s, nplo_, items[i].index);
            nplo_ += 4;
        }
        /* memcpy(b + o, b + no, s + 4); */
        /* memset(b + no, 0, s + 4); */
        /* ose_decSize(vm_s, s + 4); */
    }
    int32_t amt = ose_readSize(vm_s) - (o + s + 4);
    memmove(b + o,
            b + o + s + 4,
            amt);
    memset(b + o + amt, 0, s + 4);
    ose_decSize(vm_s, s + 4);
}

static void oscript_sort(ose_bundle osevm)
{
    oscript_sort_impl(osevm, 1, 0);
}

static void oscript_sortidxs(ose_bundle osevm)
{
    oscript_sort_impl(osevm, 0, 1);
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
    OSCRIPT_PUSHFN(sort);
    OSCRIPT_PUSHFN(sortidxs);

    ose_pushMessage(vm_s, "/o/blob/toelem",
                    strlen("/o/blob/toelem"),
                    1, OSETT_ALIGNEDPTR, oscript_blobToElem);
    OSCRIPT_PUSHFN(route);
    OSCRIPT_PUSHFN(map);
    ose_pushMessage(vm_s, "/o/maprest",
                    strlen("/o/maprest"),
                    1, OSETT_ALIGNEDPTR, oscript_mapRest);
    OSCRIPT_PUSHFN(if);
    OSCRIPT_PUSHFN(defn);
}
