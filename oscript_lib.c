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
        ose_notrot(vm_s);
        ose_drop(vm_c);     /* our command */
        ose_pushString(vm_c, "/!/o/maprest");
        ose_elemToBlob(vm_s);
        ose_moveElem(vm_s, vm_c);
        ose_moveElem(vm_s, vm_c);
        ose_dup(vm_c);
        ose_pushString(vm_c, "/!/apply");
        ose_swap(vm_c);
        ose_pushString(vm_c, "");
    }
    /* struct mapoffsets */
    /* { */
    /*     int32_t outer_bundle_size; */
    /*     char outer_bundle_id[OSE_BUNDLE_HEADER_LEN]; */
    /*     int32_t count_msg_size; */
    /*     char count_msg_addr[4]; */
    /*     char count_msg_tt[4]; */
    /*     int32_t nelems; */
    /*     int32_t count; */
    /*     int32_t idx_bundle_size; */
    /*     char idx_bundle_id[OSE_BUNDLE_HEADER_LEN]; */
    /*     int32_t idx_msg1_size; */
    /*     char idx_msg1_addr[4]; */
    /*     char idx_msg1_tt[8]; */
    /*     int32_t msg1_tt; */
    /*     int32_t msg1_offset; */
    /*     int32_t msg1_offset1; */
    /*     int32_t msg1_offset2; */
    /* } __attribute__((__packed__)); */
    /* struct mapoffsets *offsets = (struct mapoffsets *)(bs + o); */
    /* OSE_START_BUNDLE(vm_s); */
    /* const int32_t nelems = ose_ntohl(offsets->nelems); */
    /* const int32_t count = ose_ntohl(offsets->count); */
    
    /* const int32_t idx_msg_size = */
    /*     ose_ntohl(offsets->idx_msg1_size); */
    /* const int32_t idx_msg1_tt_offset = */
    /*     (char *)&(offsets->msg1_tt) - (char *)offsets; */
    /* const int32_t idx_msg1_offset_offset = */
    /*     idx_msg1_tt_offset + 4; */
    /* const int32_t idx_msg1_offset1_offset = */
    /*     idx_msg1_offset_offset + 4; */
    /* const int32_t idx_msg1_offset2_offset = */
    /*     idx_msg1_offset1_offset + 4; */
    /* for(int32_t i = 0; i < nelems; ++i) */
    /* { */
    /*     char elemtt = (char)ose_readInt32(vm_s, */
    /*                                       o + idx_msg1_tt_offset */
    /*                                       + ((idx_msg_size + 4) */
    /*                                          * i)); */
    /*     int32_t offset = ose_readInt32(vm_s, */
    /*                                    o */
    /*                                    + idx_msg1_offset_offset */
    /*                                    + ((idx_msg_size + 4) */
    /*                                       * i)); */
    /*     int32_t offset1 = ose_readInt32(vm_s, */
    /*                                     o */
    /*                                     + idx_msg1_offset1_offset */
    /*                                     + ((idx_msg_size + 4) */
    /*                                        * i)); */
    /*     if(elemtt == OSETT_BUNDLE) */
    /*     { */
    /*         ; */
    /*     } */
    /*     else */
    /*     { */
    /*         int32_t offset2 = */
    /*             ose_readInt32(vm_s, */
    /*                           o + idx_msg1_offset2_offset */
    /*                           + ((idx_msg_size + 4) */
    /*                              * i)); */
    /*         char tt = ose_readByte(vm_s, o + offset1); */
    /*         if(!tt) */
    /*         { */
    /*             ose_2drop(vm_s); */
    /*             if(i > 0) */
    /*             { */
    /*                 ose_drop(vm_s); */
    /*             } */
    /*         } */
    /*         int32_t pls = ose_getPayloadItemSize(vm_s, */
    /*                                              tt, */
    /*                                              o + offset, */
    /*                                              o + offset2); */
    /*         int32_t oldsize = ose_readSize(vm_s); */
    /*         int32_t newmsgsize = 8 + pls; */
    /*         ose_incSize(vm_s, newmsgsize + 4); */
    /*         memset(bs + oldsize, 0, newmsgsize + 4); */
    /*         ose_writeInt32(vm_s, oldsize, newmsgsize); */
    /*         bs[oldsize + 8] = OSETT_ID; */
    /*         bs[oldsize + 9] = tt; */
    /*         memcpy(bs + oldsize + 12, bs + o + offset2, pls); */

    /*         offsets->msg1_offset1 = */
    /*             ose_htonl(ose_ntohl(offsets->msg1_offset1) + 1); */
    /*         offsets->msg1_offset2 = */
    /*             ose_htonl(ose_ntohl(offsets->msg1_offset2) + pls); */
    /*     } */
    /* } */
    /* offsets->count = ose_htonl(ose_ntohl(offsets->count) + 1); */
    /* OSE_END_BUNDLE(vm_s); */
    /* { */
    /*     ose_pushBundle(vm_s); */
    /*     ose_rot(vm_s); */
    /*     ose_push(vm_s); */
    /*     ose_rot(vm_s); */
    /*     ose_dup(vm_s); */
    /*     ose_notrot(vm_s); */
    /*     ose_push(vm_s); */
    /* } */
    /* /\* drop whatever command called us *\/ */
    /* ose_drop(vm_c); */

    /* /\*  */
	/* 	/!/maprest */
	/* 	map bndl */
	/* 	/!/apply */
    /*     fn */
    /*     args */
	/* 	/s/ <-- to be dropped */
    /*  *\/ */
	/* ose_pushString(vm_c, "/!/o/maprest"); */
    /* ose_elemToBlob(vm_s); */
    /* ose_moveElem(vm_s, vm_c); */
    /* ose_pushString(vm_c, "/!/apply"); */
    /* ose_moveElem(vm_s, vm_c); */
    /* ose_moveElem(vm_s, vm_c); */
    /* /\* original command to be dropped *\/ */
    /* ose_pushString(vm_c, "");    */
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
    ose_pushMessage(vm_s, "/o/maprest",
                    strlen("/o/maprest"),
                    1, OSETT_ALIGNEDPTR, oscript_mapRest);
    OSCRIPT_PUSHFN(if);
    OSCRIPT_PUSHFN(defn);
}
