
// Generated from oscript.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "oscriptVisitor.h"


namespace oscript {

/**
 * This class provides an empty implementation of oscriptVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  oscriptBaseVisitor : public oscriptVisitor {
public:

  virtual antlrcpp::Any visitTopLevelBundle(oscriptParser::TopLevelBundleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLookupElem(oscriptParser::LookupElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncallElem(oscriptParser::FuncallElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBundleElem(oscriptParser::BundleElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMessageElem(oscriptParser::MessageElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLazyBundle(oscriptParser::LazyBundleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEagerBundle(oscriptParser::EagerBundleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnclosedLazyBundle(oscriptParser::UnclosedLazyBundleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnclosedEagerBundle(oscriptParser::UnclosedEagerBundleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBind(oscriptParser::BindContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInt(oscriptParser::IntContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFloat(oscriptParser::FloatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitString(oscriptParser::StringContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlob(oscriptParser::BlobContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitList(oscriptParser::ListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComment(oscriptParser::CommentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFn(oscriptParser::FnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArgs(oscriptParser::ArgsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFuncall(oscriptParser::FuncallContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace oscript
