#pragma once

#include "antlr4-runtime.h"
#include "oscriptVisitor.h"

#include <stdint.h>

#include "ose_conf.h"
#include "ose.h"

namespace oscript {

class  oscriptVisitorImpl : oscriptVisitor
{
private:
    ose_bundle bundle;
    int level = 0; // 0 = top-level
    int inside_funcall = 0;
    int unclosed_bundle = 0;
    void visitNonEmptyBundleImpl(std::vector<oscriptParser::OscBundleElemContext *> oscBundleElem);
    void lookupElemImpl(const char * const str);
    void visitBundle(std::vector<oscriptParser::OscBundleElemContext *> oscBundleElems);
    
public:
    ose_bundle getBundle();
    oscriptVisitorImpl(ose_bundle b);
    oscriptVisitorImpl(int32_t nbytes, char *bytes);

    void setUnclosedBundle();

    antlrcpp::Any visitTopLevelBundle(oscriptParser::TopLevelBundleContext *context);

    antlrcpp::Any visitLookupElem(oscriptParser::LookupElemContext *context);

    antlrcpp::Any visitBundleElem(oscriptParser::BundleElemContext *context);

    antlrcpp::Any visitMessageElem(oscriptParser::MessageElemContext *context);
    
    antlrcpp::Any visitFuncallElem(oscriptParser::FuncallElemContext *context);

    antlrcpp::Any visitLazyBundle(oscriptParser::LazyBundleContext *context);

    antlrcpp::Any visitEagerBundle(oscriptParser::EagerBundleContext *context);

    antlrcpp::Any visitUnclosedLazyBundle(oscriptParser::UnclosedLazyBundleContext *context);
    
    antlrcpp::Any visitUnclosedEagerBundle(oscriptParser::UnclosedEagerBundleContext *context);

    antlrcpp::Any visitBind(oscriptParser::BindContext *context);

    antlrcpp::Any visitInt(oscriptParser::IntContext *context);

    antlrcpp::Any visitFloat(oscriptParser::FloatContext *context);

    antlrcpp::Any visitString(oscriptParser::StringContext *context);

    antlrcpp::Any visitBlob(oscriptParser::BlobContext *context);

    antlrcpp::Any visitList(oscriptParser::ListContext *context);
    
    antlrcpp::Any visitComment(oscriptParser::CommentContext *context);

    antlrcpp::Any visitFn(oscriptParser::FnContext *context);

    antlrcpp::Any visitArgs(oscriptParser::ArgsContext *context);

    antlrcpp::Any visitFuncall(oscriptParser::FuncallContext *context);
};

}  // namespace oscript
