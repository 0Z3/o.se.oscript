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

#include "oscript_exec.h"
#include "oscript_lib.h"
#include "oscript_types.h"
#include "oscript_parser.h"

__attribute__((visibility("default")))
void ose_main(ose_bundle osevm)
{
	ose_bundle vm_s = OSEVM_STACK(osevm);
    
    OSE_START_BUNDLE(vm_s);
    oscript_exec_load(vm_s);
    oscript_lib_load(vm_s);
    oscript_types_load(vm_s);
    oscript_parser_load(vm_s);
    OSE_END_BUNDLE(vm_s);
}

#ifdef __cplusplus
}
#endif
