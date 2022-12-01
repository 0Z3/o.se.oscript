
// Generated from oscript.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "oscriptParser.h"


namespace oscript {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by oscriptParser.
 */
class  oscriptVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by oscriptParser.
   */
    virtual antlrcpp::Any visitTopLevelBundle(oscriptParser::TopLevelBundleContext *context) = 0;

    virtual antlrcpp::Any visitLookupElem(oscriptParser::LookupElemContext *context) = 0;

    virtual antlrcpp::Any visitFuncallElem(oscriptParser::FuncallElemContext *context) = 0;

    virtual antlrcpp::Any visitBundleElem(oscriptParser::BundleElemContext *context) = 0;

    virtual antlrcpp::Any visitMessageElem(oscriptParser::MessageElemContext *context) = 0;

    virtual antlrcpp::Any visitLazyBundle(oscriptParser::LazyBundleContext *context) = 0;

    virtual antlrcpp::Any visitEagerBundle(oscriptParser::EagerBundleContext *context) = 0;

    virtual antlrcpp::Any visitUnclosedLazyBundle(oscriptParser::UnclosedLazyBundleContext *context) = 0;

    virtual antlrcpp::Any visitUnclosedEagerBundle(oscriptParser::UnclosedEagerBundleContext *context) = 0;

    virtual antlrcpp::Any visitBind(oscriptParser::BindContext *context) = 0;

    virtual antlrcpp::Any visitInt(oscriptParser::IntContext *context) = 0;

    virtual antlrcpp::Any visitFloat(oscriptParser::FloatContext *context) = 0;

    virtual antlrcpp::Any visitString(oscriptParser::StringContext *context) = 0;

    virtual antlrcpp::Any visitBlob(oscriptParser::BlobContext *context) = 0;

    virtual antlrcpp::Any visitList(oscriptParser::ListContext *context) = 0;

    virtual antlrcpp::Any visitComment(oscriptParser::CommentContext *context) = 0;

    virtual antlrcpp::Any visitFn(oscriptParser::FnContext *context) = 0;

    virtual antlrcpp::Any visitArgs(oscriptParser::ArgsContext *context) = 0;

    virtual antlrcpp::Any visitFuncall(oscriptParser::FuncallContext *context) = 0;


};

}  // namespace oscript
