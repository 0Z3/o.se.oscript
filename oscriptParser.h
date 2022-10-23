
// Generated from oscript.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"


namespace oscript {


class  oscriptParser : public antlr4::Parser {
public:
  enum {
    Identifier = 1, OSCPattern = 2, OSCAddress = 3, Int32 = 4, Float = 5, 
    DigitSequence = 6, String = 7, Blob = 8, LParen = 9, RParen = 10, LBracket = 11, 
    RBracket = 12, LBrace = 13, RBrace = 14, SeqSep = 15, CommentLine = 16, 
    CommentMessage = 17, ClearSpace = 18, Bind = 19
  };

  enum {
    RuleTopLevelBundle = 0, RuleOscBundleElem = 1, RuleOscBundle = 2, RuleOscMessage = 3, 
    RuleFn = 4, RuleArgs = 5, RuleFuncall = 6
  };

  explicit oscriptParser(antlr4::TokenStream *input);
  ~oscriptParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class TopLevelBundleContext;
  class OscBundleElemContext;
  class OscBundleContext;
  class OscMessageContext;
  class FnContext;
  class ArgsContext;
  class FuncallContext; 

  class  TopLevelBundleContext : public antlr4::ParserRuleContext {
  public:
    oscriptParser::OscBundleElemContext *first = nullptr;
    oscriptParser::OscBundleElemContext *rest = nullptr;
    TopLevelBundleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<OscBundleElemContext *> oscBundleElem();
    OscBundleElemContext* oscBundleElem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SeqSep();
    antlr4::tree::TerminalNode* SeqSep(size_t i);
    std::vector<antlr4::tree::TerminalNode *> CommentLine();
    antlr4::tree::TerminalNode* CommentLine(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TopLevelBundleContext* topLevelBundle();

  class  OscBundleElemContext : public antlr4::ParserRuleContext {
  public:
    OscBundleElemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    OscBundleElemContext() = default;
    void copyFrom(OscBundleElemContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LookupElemContext : public OscBundleElemContext {
  public:
    LookupElemContext(OscBundleElemContext *ctx);

    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *OSCAddress();
    antlr4::tree::TerminalNode *OSCPattern();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BundleElemContext : public OscBundleElemContext {
  public:
    BundleElemContext(OscBundleElemContext *ctx);

    OscBundleContext *oscBundle();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FuncallElemContext : public OscBundleElemContext {
  public:
    FuncallElemContext(OscBundleElemContext *ctx);

    FuncallContext *funcall();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MessageElemContext : public OscBundleElemContext {
  public:
    MessageElemContext(OscBundleElemContext *ctx);

    OscMessageContext *oscMessage();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  OscBundleElemContext* oscBundleElem();

  class  OscBundleContext : public antlr4::ParserRuleContext {
  public:
    OscBundleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    OscBundleContext() = default;
    void copyFrom(OscBundleContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  EagerBundleContext : public OscBundleContext {
  public:
    EagerBundleContext(OscBundleContext *ctx);

    oscriptParser::OscBundleElemContext *first = nullptr;
    oscriptParser::OscBundleElemContext *rest = nullptr;
    antlr4::tree::TerminalNode *LParen();
    antlr4::tree::TerminalNode *RParen();
    std::vector<OscBundleElemContext *> oscBundleElem();
    OscBundleElemContext* oscBundleElem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SeqSep();
    antlr4::tree::TerminalNode* SeqSep(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LazyBundleContext : public OscBundleContext {
  public:
    LazyBundleContext(OscBundleContext *ctx);

    oscriptParser::OscBundleElemContext *first = nullptr;
    oscriptParser::OscBundleElemContext *rest = nullptr;
    antlr4::tree::TerminalNode *LBrace();
    antlr4::tree::TerminalNode *RBrace();
    std::vector<OscBundleElemContext *> oscBundleElem();
    OscBundleElemContext* oscBundleElem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SeqSep();
    antlr4::tree::TerminalNode* SeqSep(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  OscBundleContext* oscBundle();

  class  OscMessageContext : public antlr4::ParserRuleContext {
  public:
    OscMessageContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    OscMessageContext() = default;
    void copyFrom(OscMessageContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BindContext : public OscMessageContext {
  public:
    BindContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *Bind();
    OscBundleElemContext *oscBundleElem();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *OSCAddress();
    antlr4::tree::TerminalNode *OSCPattern();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BlobContext : public OscMessageContext {
  public:
    BlobContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *Blob();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  StringContext : public OscMessageContext {
  public:
    StringContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *String();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  CommentContext : public OscMessageContext {
  public:
    CommentContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *CommentMessage();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FloatContext : public OscMessageContext {
  public:
    FloatContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *Float();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ListContext : public OscMessageContext {
  public:
    ListContext(OscMessageContext *ctx);

    oscriptParser::OscBundleElemContext *first = nullptr;
    oscriptParser::OscBundleElemContext *rest = nullptr;
    antlr4::tree::TerminalNode *LBracket();
    antlr4::tree::TerminalNode *RBracket();
    std::vector<antlr4::tree::TerminalNode *> SeqSep();
    antlr4::tree::TerminalNode* SeqSep(size_t i);
    std::vector<OscBundleElemContext *> oscBundleElem();
    OscBundleElemContext* oscBundleElem(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntContext : public OscMessageContext {
  public:
    IntContext(OscMessageContext *ctx);

    antlr4::tree::TerminalNode *Int32();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  OscMessageContext* oscMessage();

  class  FnContext : public antlr4::ParserRuleContext {
  public:
    FnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *OSCAddress();
    antlr4::tree::TerminalNode *OSCPattern();
    OscBundleContext *oscBundle();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FnContext* fn();

  class  ArgsContext : public antlr4::ParserRuleContext {
  public:
    ArgsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *OSCAddress();
    antlr4::tree::TerminalNode *OSCPattern();
    OscBundleContext *oscBundle();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgsContext* args();

  class  FuncallContext : public antlr4::ParserRuleContext {
  public:
    FuncallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ArgsContext *args();
    FnContext *fn();
    FuncallContext *funcall();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FuncallContext* funcall();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace oscript
