#include <iostream>

#include "ose_conf.h"
#include "ose.h"
#include "ose_context.h"
#include "ose_util.h"
#include "ose_stackops.h"
#include "ose_print.h"
#include "ose_assert.h"

#include "oscript_types.h"

#include "antlr4-runtime.h"
#include "oscriptLexer.h"
#include "oscriptParser.h"
#include "oscriptVisitorImpl.h"

using namespace std;
using namespace antlr4;
using namespace antlrcpp;
using namespace oscript;

//#define OSCRIPTVISITORIMPL_TRACE
#ifdef OSCRIPTVISITORIMPL_TRACE
#define TRACE(c) cerr << __func__ << ":" << __LINE__ << ": " << c->getText() << "\n";
#else
#define TRACE(c)
#endif

#define OSCRIPTVISITORIMPL_LINENOS
#ifdef OSCRIPTVISITORIMPL_LINENOS

#else

#endif

oscriptVisitorImpl::oscriptVisitorImpl(ose_bundle b)
{
    bundle = b;
}

oscriptVisitorImpl::oscriptVisitorImpl(int32_t nbytes, char *bytes)
{
    bundle = ose_newBundleFromCBytes(nbytes, bytes);
}

void oscriptVisitorImpl::setUnclosedBundle()
{
    unclosed_bundle = 1;
}
    
antlrcpp::Any
oscriptVisitorImpl::visitTopLevelBundle(oscriptParser::TopLevelBundleContext *context)
{
 	TRACE(context);
    const int32_t s1 = ose_readSize(bundle);
    ose_pushBundle(bundle);
    
    Any out = visitChildren(context);
    const int32_t s2 = ose_readSize(bundle);
    ose_writeInt32(bundle, s1, s2 - s1 - 4);
    {
        ose_pushBundle(bundle);
        ose_swap(bundle);
        ose_push(bundle);
    }
    return out;
}

void oscriptVisitorImpl::lookupElemImpl(const char * const str)
{
    ose_pushString(bundle, str);
    ose_pushString(bundle, "/!/$");
    ose_push(bundle);
}

antlrcpp::Any
oscriptVisitorImpl::visitLookupElem(oscriptParser::LookupElemContext *context)
{
 	TRACE(context);
    lookupElemImpl(context->getText().c_str());
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitBundleElem(oscriptParser::BundleElemContext *context)
{
 	TRACE(context);
    visitChildren(context);
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitMessageElem(oscriptParser::MessageElemContext *context)
{
 	TRACE(context);
    visitChildren(context);
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitFuncallElem(oscriptParser::FuncallElemContext *context)
{
 	TRACE(context);
    return visit(context->funcall());
}

antlrcpp::Any
oscriptVisitorImpl::visitBind(oscriptParser::BindContext *context)
{
    TRACE(context);
                    
    // if(level == 0)
    {
        // printf("line: %zu\n", context->getStart()->getLine());
        // char buf[256];
        // sprintf(buf, "%zu", context->getStart()->getLine());
        // ose_pushMessage(bundle,
        //                 "/line",
        //                 strlen("/line"),
        //                 1,
        //                 OSETT_INT32, context->getStart()->getLine());

    }

    bool haverhs = false;
    if(context->oscBundleElem())
    {
        haverhs = true;
        visit(context->oscBundleElem());
        if(ose_peekType(bundle) == OSETT_BUNDLE)
        {
            ose_elemToBlob(bundle);
            ose_setTypetag_impl(bundle, OSETT_BUNDLE);
        }
    }

    if(context->OSCAddress())
    {
        ose_pushString(bundle,
                       context->OSCAddress()->getText().c_str());
    }
    else if(context->OSCPattern())
    {
        ose_pushString(bundle,
                       context->OSCPattern()->getText().c_str());
    }
    else if(context->Identifier())
    {
        ose_pushString(bundle,
                       context->Identifier()->getText().c_str());
    }
    else
    {
        ose_pushString(bundle, OSE_ADDRESS_ANONVAL);
    }
    if(haverhs)
    {
    	ose_push(bundle);
    }
    ose_moveStringToAddress(bundle);
    if(level == 0)
    {
        // top-level bind:
        // turn this message into an assignment type
        // ose_elemToBlob(bundle);
        // ose_setTypetag_impl(bundle, OSCRIPT_TYPETAG_ASSIGN);
    }
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitInt(oscriptParser::IntContext *context)
{
	TRACE(context);
    ose_pushInt32(bundle, std::stol(context->getText()));
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitFloat(oscriptParser::FloatContext *context)
{
	TRACE(context);
    ose_pushFloat(bundle, std::stof(context->getText()));
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitString(oscriptParser::StringContext *context)
{
	TRACE(context);
    const std::string str = context->getText();
    const int32_t len = str.length();
    const std::string strq = str.substr(1, len - 2);
    // process escapes here
    ose_pushString(bundle, strq.c_str());
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitBlob(oscriptParser::BlobContext *context)
{
	TRACE(context);
    std::string str = context->getText();
    const int32_t n = str.length() - 1;
    ose_assert(n % 2 == 0);
    ose_pushBlob(bundle, n / 2, NULL);
    char *bp = ose_peekBlob(bundle);
    bp += 4;
    for(int i = 1; i < n + 1; i += 2)
    {
        char buf[3] = {str[i], str[i + 1], 0};
        char c = (char)strtol(buf, NULL, 16);
        *bp++ = c;
    }
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitList(oscriptParser::ListContext *context)
{
	TRACE(context);
    if(context->oscBundleElem().size() == 0)
    {
        ose_pushMessage(bundle, OSE_ADDRESS_ANONVAL,
                        OSE_ADDRESS_ANONVAL_LEN, 0);
        return context;
    }
    visit(context->oscBundleElem()[0]);
    for(int i = 1; i < context->oscBundleElem().size(); ++i)
    {
        visit(context->oscBundleElem()[i]);
        ose_push(bundle);
    }
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitComment(oscriptParser::CommentContext *context)
{
    TRACE(context);
    ose_pushString(bundle, context->getText().c_str());
    return context;
}

void oscriptVisitorImpl::visitBundle(std::vector<oscriptParser::OscBundleElemContext *> oscBundleElems)
{
    OSE_START_BUNDLE(bundle);
    ++level;
    {
        for(int i = 0; i < oscBundleElems.size(); ++i)
        {
            auto e = oscBundleElems[i];
            visit(e);
        }
    }
    --level;
    OSE_END_BUNDLE(bundle);
}

antlrcpp::Any
oscriptVisitorImpl::visitLazyBundle(oscriptParser::LazyBundleContext *context)
{
 	TRACE(context);
    // ++level;
    // ose_pushBundle(bundle);
    // int32_t o = ose_getLastBundleElemOffset(bundle);
    // int32_t bs = ose_readSize(bundle);
    // {
    //     for(int i = 0; i < context->oscBundleElem().size(); ++i)
    //     {
    //         auto e = context->oscBundleElem()[i];
    //         visit(e);
    //     }
    // }
    
    // int32_t nbs = ose_readSize(bundle);
    // ose_writeInt32(bundle, o, OSE_BUNDLE_HEADER_LEN + (nbs - bs));
    visitBundle(context->oscBundleElem());
    ose_elemToBlob(bundle);
    // --level;
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitEagerBundle(oscriptParser::EagerBundleContext *context)
{
 	TRACE(context);
    // ++level;
    // ose_pushBundle(bundle);
    // int32_t o = ose_getLastBundleElemOffset(bundle);
    // int32_t bs = ose_readSize(bundle);
    // {
    //     for(int i = 0; i < context->oscBundleElem().size(); ++i)
    //     {
    //         auto e = context->oscBundleElem()[i];
    //         visit(e);
    //     }
    // }
    
    // int32_t nbs = ose_readSize(bundle);
    // ose_writeInt32(bundle, o, OSE_BUNDLE_HEADER_LEN + (nbs - bs));
    // --level;
    visitBundle(context->oscBundleElem());
    return context;
}

void postErrorMessage(std::string text)
{
    int n = printf("%s\n", text.c_str());
    for(int i = 0; i < n - 1; ++i)
    {
        printf("~");
    }
    printf("^\n");
}

antlrcpp::Any
oscriptVisitorImpl::visitUnclosedLazyBundle(oscriptParser::UnclosedLazyBundleContext *context)
{
    TRACE(context);
    printf("level: %d\n", level);
    postErrorMessage(context->getText());
    visitBundle(context->oscBundleElem());
    ose_elemToBlob(bundle);
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitUnclosedEagerBundle(oscriptParser::UnclosedEagerBundleContext *context)
{
    TRACE(context);
    printf("level: %d\n", level);
    postErrorMessage(context->getText());
    visitBundle(context->oscBundleElem());
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitFn(oscriptParser::FnContext *context)
{
    TRACE(context);
    if(context->Identifier())
    {
        lookupElemImpl(context->Identifier()->getText().c_str());
    }
    else if(context->OSCAddress())
    {
        lookupElemImpl(context->OSCAddress()->getText().c_str());
    }
    else if(context->OSCPattern())
    {
        lookupElemImpl(context->OSCPattern()->getText().c_str());
    }
    else if(context->oscBundle())
    {
        visit(context->oscBundle());
        // ose_elemToBlob(bundle);
        // ose_setTypetag_impl(bundle, OSETT_BUNDLE);
    }
    ose_pushString(bundle, "/!/o/apply");
    ose_push(bundle);
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitArgs(oscriptParser::ArgsContext *context)
{
    TRACE(context);
    if(context->Identifier())
    {
        lookupElemImpl(context->Identifier()->getText().c_str());
    }
    else if(context->OSCAddress())
    {
        lookupElemImpl(context->OSCAddress()->getText().c_str());
    }
    else if(context->OSCPattern())
    {
        lookupElemImpl(context->OSCPattern()->getText().c_str());
    }
    else if(context->oscBundle())
    {
        visit(context->oscBundle());
        ose_elemToBlob(bundle);
        ose_setTypetag_impl(bundle, OSETT_BUNDLE);
    }
    return context;
}

antlrcpp::Any
oscriptVisitorImpl::visitFuncall(oscriptParser::FuncallContext *context)
{
    TRACE(context);
    inside_funcall = 1;
    // if(context->args())
    {
    	visit(context->args());
    }
    if(context->fn())
    {
        visit(context->fn());
    }
    else if(context->funcall())
    {
        visit(context->funcall());
        ose_pushString(bundle, "/!/o/apply");
    	ose_push(bundle);
    }
    else
    {
        ose_assert(0);
    }
    // visit(context->fn());
    ose_push(bundle);
    inside_funcall = 0;
    return context;
}
