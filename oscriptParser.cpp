
// Generated from oscript.g4 by ANTLR 4.9.3


#include "oscriptVisitor.h"

#include "oscriptParser.h"


using namespace antlrcpp;
using namespace oscript;
using namespace antlr4;

oscriptParser::oscriptParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

oscriptParser::~oscriptParser() {
  delete _interpreter;
}

std::string oscriptParser::getGrammarFileName() const {
  return "oscript.g4";
}

const std::vector<std::string>& oscriptParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& oscriptParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- TopLevelBundleContext ------------------------------------------------------------------

oscriptParser::TopLevelBundleContext::TopLevelBundleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* oscriptParser::TopLevelBundleContext::EOF() {
  return getToken(oscriptParser::EOF, 0);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::TopLevelBundleContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::TopLevelBundleContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

std::vector<tree::TerminalNode *> oscriptParser::TopLevelBundleContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::TopLevelBundleContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

std::vector<tree::TerminalNode *> oscriptParser::TopLevelBundleContext::CommentLine() {
  return getTokens(oscriptParser::CommentLine);
}

tree::TerminalNode* oscriptParser::TopLevelBundleContext::CommentLine(size_t i) {
  return getToken(oscriptParser::CommentLine, i);
}


size_t oscriptParser::TopLevelBundleContext::getRuleIndex() const {
  return oscriptParser::RuleTopLevelBundle;
}


antlrcpp::Any oscriptParser::TopLevelBundleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitTopLevelBundle(this);
  else
    return visitor->visitChildren(this);
}

oscriptParser::TopLevelBundleContext* oscriptParser::topLevelBundle() {
  TopLevelBundleContext *_localctx = _tracker.createInstance<TopLevelBundleContext>(_ctx, getState());
  enterRule(_localctx, 0, oscriptParser::RuleTopLevelBundle);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(34);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case oscriptParser::Identifier:
      case oscriptParser::OSCPattern:
      case oscriptParser::OSCAddress:
      case oscriptParser::Int32:
      case oscriptParser::Float:
      case oscriptParser::String:
      case oscriptParser::Blob:
      case oscriptParser::LParen:
      case oscriptParser::LBracket:
      case oscriptParser::LBrace:
      case oscriptParser::CommentMessage:
      case oscriptParser::Bind: {
        enterOuterAlt(_localctx, 1);
        setState(14);
        antlrcpp::downCast<TopLevelBundleContext *>(_localctx)->first = oscBundleElem();
        setState(19);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(15);
            match(oscriptParser::SeqSep);
            setState(16);
            antlrcpp::downCast<TopLevelBundleContext *>(_localctx)->rest = oscBundleElem(); 
          }
          setState(21);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
        }
        setState(23);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == oscriptParser::SeqSep) {
          setState(22);
          match(oscriptParser::SeqSep);
        }
        setState(25);
        match(oscriptParser::EOF);
        break;
      }

      case oscriptParser::EOF:
      case oscriptParser::CommentLine: {
        enterOuterAlt(_localctx, 2);
        setState(30);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == oscriptParser::CommentLine) {
          setState(27);
          match(oscriptParser::CommentLine);
          setState(32);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(33);
        match(oscriptParser::EOF);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OscBundleElemContext ------------------------------------------------------------------

oscriptParser::OscBundleElemContext::OscBundleElemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t oscriptParser::OscBundleElemContext::getRuleIndex() const {
  return oscriptParser::RuleOscBundleElem;
}

void oscriptParser::OscBundleElemContext::copyFrom(OscBundleElemContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LookupElemContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::LookupElemContext::Identifier() {
  return getToken(oscriptParser::Identifier, 0);
}

tree::TerminalNode* oscriptParser::LookupElemContext::OSCAddress() {
  return getToken(oscriptParser::OSCAddress, 0);
}

tree::TerminalNode* oscriptParser::LookupElemContext::OSCPattern() {
  return getToken(oscriptParser::OSCPattern, 0);
}

oscriptParser::LookupElemContext::LookupElemContext(OscBundleElemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::LookupElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitLookupElem(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BundleElemContext ------------------------------------------------------------------

oscriptParser::OscBundleContext* oscriptParser::BundleElemContext::oscBundle() {
  return getRuleContext<oscriptParser::OscBundleContext>(0);
}

oscriptParser::BundleElemContext::BundleElemContext(OscBundleElemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::BundleElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitBundleElem(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FuncallElemContext ------------------------------------------------------------------

oscriptParser::FuncallContext* oscriptParser::FuncallElemContext::funcall() {
  return getRuleContext<oscriptParser::FuncallContext>(0);
}

oscriptParser::FuncallElemContext::FuncallElemContext(OscBundleElemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::FuncallElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitFuncallElem(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MessageElemContext ------------------------------------------------------------------

oscriptParser::OscMessageContext* oscriptParser::MessageElemContext::oscMessage() {
  return getRuleContext<oscriptParser::OscMessageContext>(0);
}

oscriptParser::MessageElemContext::MessageElemContext(OscBundleElemContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::MessageElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitMessageElem(this);
  else
    return visitor->visitChildren(this);
}
oscriptParser::OscBundleElemContext* oscriptParser::oscBundleElem() {
  OscBundleElemContext *_localctx = _tracker.createInstance<OscBundleElemContext>(_ctx, getState());
  enterRule(_localctx, 2, oscriptParser::RuleOscBundleElem);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(40);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<oscriptParser::LookupElemContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(36);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
        | (1ULL << oscriptParser::OSCPattern)
        | (1ULL << oscriptParser::OSCAddress))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<oscriptParser::FuncallElemContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(37);
      funcall();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<oscriptParser::BundleElemContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(38);
      oscBundle();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<oscriptParser::MessageElemContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(39);
      oscMessage();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OscBundleContext ------------------------------------------------------------------

oscriptParser::OscBundleContext::OscBundleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t oscriptParser::OscBundleContext::getRuleIndex() const {
  return oscriptParser::RuleOscBundle;
}

void oscriptParser::OscBundleContext::copyFrom(OscBundleContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- UnclosedEagerBundleContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::UnclosedEagerBundleContext::LParen() {
  return getToken(oscriptParser::LParen, 0);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::UnclosedEagerBundleContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::UnclosedEagerBundleContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

std::vector<tree::TerminalNode *> oscriptParser::UnclosedEagerBundleContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::UnclosedEagerBundleContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

oscriptParser::UnclosedEagerBundleContext::UnclosedEagerBundleContext(OscBundleContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::UnclosedEagerBundleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitUnclosedEagerBundle(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EagerBundleContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::EagerBundleContext::LParen() {
  return getToken(oscriptParser::LParen, 0);
}

tree::TerminalNode* oscriptParser::EagerBundleContext::RParen() {
  return getToken(oscriptParser::RParen, 0);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::EagerBundleContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::EagerBundleContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

std::vector<tree::TerminalNode *> oscriptParser::EagerBundleContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::EagerBundleContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

oscriptParser::EagerBundleContext::EagerBundleContext(OscBundleContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::EagerBundleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitEagerBundle(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LazyBundleContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::LazyBundleContext::LBrace() {
  return getToken(oscriptParser::LBrace, 0);
}

tree::TerminalNode* oscriptParser::LazyBundleContext::RBrace() {
  return getToken(oscriptParser::RBrace, 0);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::LazyBundleContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::LazyBundleContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

std::vector<tree::TerminalNode *> oscriptParser::LazyBundleContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::LazyBundleContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

oscriptParser::LazyBundleContext::LazyBundleContext(OscBundleContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::LazyBundleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitLazyBundle(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnclosedLazyBundleContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::UnclosedLazyBundleContext::LBrace() {
  return getToken(oscriptParser::LBrace, 0);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::UnclosedLazyBundleContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::UnclosedLazyBundleContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

std::vector<tree::TerminalNode *> oscriptParser::UnclosedLazyBundleContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::UnclosedLazyBundleContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

oscriptParser::UnclosedLazyBundleContext::UnclosedLazyBundleContext(OscBundleContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::UnclosedLazyBundleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitUnclosedLazyBundle(this);
  else
    return visitor->visitChildren(this);
}
oscriptParser::OscBundleContext* oscriptParser::oscBundle() {
  OscBundleContext *_localctx = _tracker.createInstance<OscBundleContext>(_ctx, getState());
  enterRule(_localctx, 4, oscriptParser::RuleOscBundle);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(96);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<oscriptParser::LazyBundleContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(42);
      match(oscriptParser::LBrace);
      setState(54);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
        | (1ULL << oscriptParser::OSCPattern)
        | (1ULL << oscriptParser::OSCAddress)
        | (1ULL << oscriptParser::Int32)
        | (1ULL << oscriptParser::Float)
        | (1ULL << oscriptParser::String)
        | (1ULL << oscriptParser::Blob)
        | (1ULL << oscriptParser::LParen)
        | (1ULL << oscriptParser::LBracket)
        | (1ULL << oscriptParser::LBrace)
        | (1ULL << oscriptParser::CommentMessage)
        | (1ULL << oscriptParser::Bind))) != 0)) {
        setState(43);
        antlrcpp::downCast<LazyBundleContext *>(_localctx)->first = oscBundleElem();
        setState(48);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(44);
            match(oscriptParser::SeqSep);
            setState(45);
            antlrcpp::downCast<LazyBundleContext *>(_localctx)->rest = oscBundleElem(); 
          }
          setState(50);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
        }
        setState(52);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == oscriptParser::SeqSep) {
          setState(51);
          match(oscriptParser::SeqSep);
        }
      }
      setState(56);
      match(oscriptParser::RBrace);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<oscriptParser::EagerBundleContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(57);
      match(oscriptParser::LParen);
      setState(69);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
        | (1ULL << oscriptParser::OSCPattern)
        | (1ULL << oscriptParser::OSCAddress)
        | (1ULL << oscriptParser::Int32)
        | (1ULL << oscriptParser::Float)
        | (1ULL << oscriptParser::String)
        | (1ULL << oscriptParser::Blob)
        | (1ULL << oscriptParser::LParen)
        | (1ULL << oscriptParser::LBracket)
        | (1ULL << oscriptParser::LBrace)
        | (1ULL << oscriptParser::CommentMessage)
        | (1ULL << oscriptParser::Bind))) != 0)) {
        setState(58);
        antlrcpp::downCast<EagerBundleContext *>(_localctx)->first = oscBundleElem();
        setState(63);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(59);
            match(oscriptParser::SeqSep);
            setState(60);
            antlrcpp::downCast<EagerBundleContext *>(_localctx)->rest = oscBundleElem(); 
          }
          setState(65);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
        }
        setState(67);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == oscriptParser::SeqSep) {
          setState(66);
          match(oscriptParser::SeqSep);
        }
      }
      setState(71);
      match(oscriptParser::RParen);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<oscriptParser::UnclosedLazyBundleContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(72);
      match(oscriptParser::LBrace);
      setState(81);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
      case 1: {
        setState(73);
        antlrcpp::downCast<UnclosedLazyBundleContext *>(_localctx)->first = oscBundleElem();
        setState(78);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(74);
            match(oscriptParser::SeqSep);
            setState(75);
            antlrcpp::downCast<UnclosedLazyBundleContext *>(_localctx)->rest = oscBundleElem(); 
          }
          setState(80);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
        }
        break;
      }

      default:
        break;
      }

                  notifyErrorListeners(_input->LT(-1),
                                       "Unclosed bundle: missing '}'",
                                       NULL);
              
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<oscriptParser::UnclosedEagerBundleContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(84);
      match(oscriptParser::LParen);
      setState(93);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
      case 1: {
        setState(85);
        antlrcpp::downCast<UnclosedEagerBundleContext *>(_localctx)->first = oscBundleElem();
        setState(90);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
          if (alt == 1) {
            setState(86);
            match(oscriptParser::SeqSep);
            setState(87);
            antlrcpp::downCast<UnclosedEagerBundleContext *>(_localctx)->rest = oscBundleElem(); 
          }
          setState(92);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
        }
        break;
      }

      default:
        break;
      }

                  notifyErrorListeners(_input->LT(-1),
                                       "Unclosed bundle: missing ')'",
                                       NULL);
              
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OscMessageContext ------------------------------------------------------------------

oscriptParser::OscMessageContext::OscMessageContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t oscriptParser::OscMessageContext::getRuleIndex() const {
  return oscriptParser::RuleOscMessage;
}

void oscriptParser::OscMessageContext::copyFrom(OscMessageContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- BindContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::BindContext::Bind() {
  return getToken(oscriptParser::Bind, 0);
}

oscriptParser::OscBundleElemContext* oscriptParser::BindContext::oscBundleElem() {
  return getRuleContext<oscriptParser::OscBundleElemContext>(0);
}

tree::TerminalNode* oscriptParser::BindContext::Identifier() {
  return getToken(oscriptParser::Identifier, 0);
}

tree::TerminalNode* oscriptParser::BindContext::OSCAddress() {
  return getToken(oscriptParser::OSCAddress, 0);
}

tree::TerminalNode* oscriptParser::BindContext::OSCPattern() {
  return getToken(oscriptParser::OSCPattern, 0);
}

oscriptParser::BindContext::BindContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::BindContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitBind(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlobContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::BlobContext::Blob() {
  return getToken(oscriptParser::Blob, 0);
}

oscriptParser::BlobContext::BlobContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::BlobContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitBlob(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StringContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::StringContext::String() {
  return getToken(oscriptParser::String, 0);
}

oscriptParser::StringContext::StringContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CommentContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::CommentContext::CommentMessage() {
  return getToken(oscriptParser::CommentMessage, 0);
}

oscriptParser::CommentContext::CommentContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::CommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitComment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FloatContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::FloatContext::Float() {
  return getToken(oscriptParser::Float, 0);
}

oscriptParser::FloatContext::FloatContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::FloatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitFloat(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ListContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::ListContext::LBracket() {
  return getToken(oscriptParser::LBracket, 0);
}

tree::TerminalNode* oscriptParser::ListContext::RBracket() {
  return getToken(oscriptParser::RBracket, 0);
}

std::vector<tree::TerminalNode *> oscriptParser::ListContext::SeqSep() {
  return getTokens(oscriptParser::SeqSep);
}

tree::TerminalNode* oscriptParser::ListContext::SeqSep(size_t i) {
  return getToken(oscriptParser::SeqSep, i);
}

std::vector<oscriptParser::OscBundleElemContext *> oscriptParser::ListContext::oscBundleElem() {
  return getRuleContexts<oscriptParser::OscBundleElemContext>();
}

oscriptParser::OscBundleElemContext* oscriptParser::ListContext::oscBundleElem(size_t i) {
  return getRuleContext<oscriptParser::OscBundleElemContext>(i);
}

oscriptParser::ListContext::ListContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::ListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitList(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntContext ------------------------------------------------------------------

tree::TerminalNode* oscriptParser::IntContext::Int32() {
  return getToken(oscriptParser::Int32, 0);
}

oscriptParser::IntContext::IntContext(OscMessageContext *ctx) { copyFrom(ctx); }


antlrcpp::Any oscriptParser::IntContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitInt(this);
  else
    return visitor->visitChildren(this);
}
oscriptParser::OscMessageContext* oscriptParser::oscMessage() {
  OscMessageContext *_localctx = _tracker.createInstance<OscMessageContext>(_ctx, getState());
  enterRule(_localctx, 6, oscriptParser::RuleOscMessage);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(122);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case oscriptParser::Identifier:
      case oscriptParser::OSCPattern:
      case oscriptParser::OSCAddress:
      case oscriptParser::Bind: {
        _localctx = _tracker.createInstance<oscriptParser::BindContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(99);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
          | (1ULL << oscriptParser::OSCPattern)
          | (1ULL << oscriptParser::OSCAddress))) != 0)) {
          setState(98);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
            | (1ULL << oscriptParser::OSCPattern)
            | (1ULL << oscriptParser::OSCAddress))) != 0))) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
        }
        setState(101);
        match(oscriptParser::Bind);
        setState(103);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
        case 1: {
          setState(102);
          oscBundleElem();
          break;
        }

        default:
          break;
        }
        break;
      }

      case oscriptParser::Int32: {
        _localctx = _tracker.createInstance<oscriptParser::IntContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(105);
        match(oscriptParser::Int32);
        break;
      }

      case oscriptParser::Float: {
        _localctx = _tracker.createInstance<oscriptParser::FloatContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(106);
        match(oscriptParser::Float);
        break;
      }

      case oscriptParser::String: {
        _localctx = _tracker.createInstance<oscriptParser::StringContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(107);
        match(oscriptParser::String);
        break;
      }

      case oscriptParser::Blob: {
        _localctx = _tracker.createInstance<oscriptParser::BlobContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(108);
        match(oscriptParser::Blob);
        break;
      }

      case oscriptParser::LBracket: {
        _localctx = _tracker.createInstance<oscriptParser::ListContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(109);
        match(oscriptParser::LBracket);
        setState(111);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << oscriptParser::Identifier)
          | (1ULL << oscriptParser::OSCPattern)
          | (1ULL << oscriptParser::OSCAddress)
          | (1ULL << oscriptParser::Int32)
          | (1ULL << oscriptParser::Float)
          | (1ULL << oscriptParser::String)
          | (1ULL << oscriptParser::Blob)
          | (1ULL << oscriptParser::LParen)
          | (1ULL << oscriptParser::LBracket)
          | (1ULL << oscriptParser::LBrace)
          | (1ULL << oscriptParser::CommentMessage)
          | (1ULL << oscriptParser::Bind))) != 0)) {
          setState(110);
          antlrcpp::downCast<ListContext *>(_localctx)->first = oscBundleElem();
        }
        setState(117);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == oscriptParser::SeqSep) {
          setState(113);
          match(oscriptParser::SeqSep);
          setState(114);
          antlrcpp::downCast<ListContext *>(_localctx)->rest = oscBundleElem();
          setState(119);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(120);
        match(oscriptParser::RBracket);
        break;
      }

      case oscriptParser::CommentMessage: {
        _localctx = _tracker.createInstance<oscriptParser::CommentContext>(_localctx);
        enterOuterAlt(_localctx, 7);
        setState(121);
        match(oscriptParser::CommentMessage);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FnContext ------------------------------------------------------------------

oscriptParser::FnContext::FnContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* oscriptParser::FnContext::Identifier() {
  return getToken(oscriptParser::Identifier, 0);
}

tree::TerminalNode* oscriptParser::FnContext::OSCAddress() {
  return getToken(oscriptParser::OSCAddress, 0);
}

tree::TerminalNode* oscriptParser::FnContext::OSCPattern() {
  return getToken(oscriptParser::OSCPattern, 0);
}

oscriptParser::OscBundleContext* oscriptParser::FnContext::oscBundle() {
  return getRuleContext<oscriptParser::OscBundleContext>(0);
}


size_t oscriptParser::FnContext::getRuleIndex() const {
  return oscriptParser::RuleFn;
}


antlrcpp::Any oscriptParser::FnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitFn(this);
  else
    return visitor->visitChildren(this);
}

oscriptParser::FnContext* oscriptParser::fn() {
  FnContext *_localctx = _tracker.createInstance<FnContext>(_ctx, getState());
  enterRule(_localctx, 8, oscriptParser::RuleFn);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(128);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case oscriptParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(124);
        match(oscriptParser::Identifier);
        break;
      }

      case oscriptParser::OSCAddress: {
        enterOuterAlt(_localctx, 2);
        setState(125);
        match(oscriptParser::OSCAddress);
        break;
      }

      case oscriptParser::OSCPattern: {
        enterOuterAlt(_localctx, 3);
        setState(126);
        match(oscriptParser::OSCPattern);
        break;
      }

      case oscriptParser::LParen:
      case oscriptParser::LBrace: {
        enterOuterAlt(_localctx, 4);
        setState(127);
        oscBundle();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgsContext ------------------------------------------------------------------

oscriptParser::ArgsContext::ArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* oscriptParser::ArgsContext::Identifier() {
  return getToken(oscriptParser::Identifier, 0);
}

tree::TerminalNode* oscriptParser::ArgsContext::OSCAddress() {
  return getToken(oscriptParser::OSCAddress, 0);
}

tree::TerminalNode* oscriptParser::ArgsContext::OSCPattern() {
  return getToken(oscriptParser::OSCPattern, 0);
}

oscriptParser::OscBundleContext* oscriptParser::ArgsContext::oscBundle() {
  return getRuleContext<oscriptParser::OscBundleContext>(0);
}


size_t oscriptParser::ArgsContext::getRuleIndex() const {
  return oscriptParser::RuleArgs;
}


antlrcpp::Any oscriptParser::ArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitArgs(this);
  else
    return visitor->visitChildren(this);
}

oscriptParser::ArgsContext* oscriptParser::args() {
  ArgsContext *_localctx = _tracker.createInstance<ArgsContext>(_ctx, getState());
  enterRule(_localctx, 10, oscriptParser::RuleArgs);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(134);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case oscriptParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(130);
        match(oscriptParser::Identifier);
        break;
      }

      case oscriptParser::OSCAddress: {
        enterOuterAlt(_localctx, 2);
        setState(131);
        match(oscriptParser::OSCAddress);
        break;
      }

      case oscriptParser::OSCPattern: {
        enterOuterAlt(_localctx, 3);
        setState(132);
        match(oscriptParser::OSCPattern);
        break;
      }

      case oscriptParser::LParen:
      case oscriptParser::LBrace: {
        enterOuterAlt(_localctx, 4);
        setState(133);
        oscBundle();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncallContext ------------------------------------------------------------------

oscriptParser::FuncallContext::FuncallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

oscriptParser::ArgsContext* oscriptParser::FuncallContext::args() {
  return getRuleContext<oscriptParser::ArgsContext>(0);
}

oscriptParser::FnContext* oscriptParser::FuncallContext::fn() {
  return getRuleContext<oscriptParser::FnContext>(0);
}

oscriptParser::FuncallContext* oscriptParser::FuncallContext::funcall() {
  return getRuleContext<oscriptParser::FuncallContext>(0);
}


size_t oscriptParser::FuncallContext::getRuleIndex() const {
  return oscriptParser::RuleFuncall;
}


antlrcpp::Any oscriptParser::FuncallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<oscriptVisitor*>(visitor))
    return parserVisitor->visitFuncall(this);
  else
    return visitor->visitChildren(this);
}

oscriptParser::FuncallContext* oscriptParser::funcall() {
  FuncallContext *_localctx = _tracker.createInstance<FuncallContext>(_ctx, getState());
  enterRule(_localctx, 12, oscriptParser::RuleFuncall);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(142);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(136);
      args();
      setState(137);
      fn();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(139);
      args();
      setState(140);
      funcall();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> oscriptParser::_decisionToDFA;
atn::PredictionContextCache oscriptParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN oscriptParser::_atn;
std::vector<uint16_t> oscriptParser::_serializedATN;

std::vector<std::string> oscriptParser::_ruleNames = {
  "topLevelBundle", "oscBundleElem", "oscBundle", "oscMessage", "fn", "args", 
  "funcall"
};

std::vector<std::string> oscriptParser::_literalNames = {
  "", "", "", "", "", "", "", "", "", "'('", "')'", "'['", "']'", "'{'", 
  "'}'", "','", "", "", "", "':'"
};

std::vector<std::string> oscriptParser::_symbolicNames = {
  "", "Identifier", "OSCPattern", "OSCAddress", "Int32", "Float", "DigitSequence", 
  "String", "Blob", "LParen", "RParen", "LBracket", "RBracket", "LBrace", 
  "RBrace", "SeqSep", "CommentLine", "CommentMessage", "ClearSpace", "Bind"
};

dfa::Vocabulary oscriptParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> oscriptParser::_tokenNames;

oscriptParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x15, 0x93, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 
       0x14, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x17, 0xb, 0x2, 0x3, 0x2, 0x5, 
       0x2, 0x1a, 0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x1f, 
       0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x22, 0xb, 0x2, 0x3, 0x2, 0x5, 0x2, 
       0x25, 0xa, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 
       0x2b, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 
       0x31, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x34, 0xb, 0x4, 0x3, 0x4, 0x5, 
       0x4, 0x37, 0xa, 0x4, 0x5, 0x4, 0x39, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x40, 0xa, 0x4, 0xc, 0x4, 
       0xe, 0x4, 0x43, 0xb, 0x4, 0x3, 0x4, 0x5, 0x4, 0x46, 0xa, 0x4, 0x5, 
       0x4, 0x48, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
       0x4, 0x7, 0x4, 0x4f, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x52, 0xb, 0x4, 
       0x5, 0x4, 0x54, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
       0x3, 0x4, 0x7, 0x4, 0x5b, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x5e, 0xb, 
       0x4, 0x5, 0x4, 0x60, 0xa, 0x4, 0x3, 0x4, 0x5, 0x4, 0x63, 0xa, 0x4, 
       0x3, 0x5, 0x5, 0x5, 0x66, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 
       0x6a, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x5, 0x5, 0x5, 0x72, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 
       0x76, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x79, 0xb, 0x5, 0x3, 0x5, 0x3, 
       0x5, 0x5, 0x5, 0x7d, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x5, 0x6, 0x83, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
       0x7, 0x5, 0x7, 0x89, 0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x91, 0xa, 0x8, 0x3, 0x8, 0x2, 
       0x2, 0x9, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x2, 0x3, 0x3, 0x2, 
       0x3, 0x5, 0x2, 0xb0, 0x2, 0x24, 0x3, 0x2, 0x2, 0x2, 0x4, 0x2a, 0x3, 
       0x2, 0x2, 0x2, 0x6, 0x62, 0x3, 0x2, 0x2, 0x2, 0x8, 0x7c, 0x3, 0x2, 
       0x2, 0x2, 0xa, 0x82, 0x3, 0x2, 0x2, 0x2, 0xc, 0x88, 0x3, 0x2, 0x2, 
       0x2, 0xe, 0x90, 0x3, 0x2, 0x2, 0x2, 0x10, 0x15, 0x5, 0x4, 0x3, 0x2, 
       0x11, 0x12, 0x7, 0x11, 0x2, 0x2, 0x12, 0x14, 0x5, 0x4, 0x3, 0x2, 
       0x13, 0x11, 0x3, 0x2, 0x2, 0x2, 0x14, 0x17, 0x3, 0x2, 0x2, 0x2, 0x15, 
       0x13, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x3, 0x2, 0x2, 0x2, 0x16, 0x19, 
       0x3, 0x2, 0x2, 0x2, 0x17, 0x15, 0x3, 0x2, 0x2, 0x2, 0x18, 0x1a, 0x7, 
       0x11, 0x2, 0x2, 0x19, 0x18, 0x3, 0x2, 0x2, 0x2, 0x19, 0x1a, 0x3, 
       0x2, 0x2, 0x2, 0x1a, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x1c, 0x7, 0x2, 
       0x2, 0x3, 0x1c, 0x25, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1f, 0x7, 0x12, 
       0x2, 0x2, 0x1e, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x22, 0x3, 0x2, 0x2, 
       0x2, 0x20, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x3, 0x2, 0x2, 0x2, 
       0x21, 0x23, 0x3, 0x2, 0x2, 0x2, 0x22, 0x20, 0x3, 0x2, 0x2, 0x2, 0x23, 
       0x25, 0x7, 0x2, 0x2, 0x3, 0x24, 0x10, 0x3, 0x2, 0x2, 0x2, 0x24, 0x20, 
       0x3, 0x2, 0x2, 0x2, 0x25, 0x3, 0x3, 0x2, 0x2, 0x2, 0x26, 0x2b, 0x9, 
       0x2, 0x2, 0x2, 0x27, 0x2b, 0x5, 0xe, 0x8, 0x2, 0x28, 0x2b, 0x5, 0x6, 
       0x4, 0x2, 0x29, 0x2b, 0x5, 0x8, 0x5, 0x2, 0x2a, 0x26, 0x3, 0x2, 0x2, 
       0x2, 0x2a, 0x27, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x28, 0x3, 0x2, 0x2, 0x2, 
       0x2a, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2c, 
       0x38, 0x7, 0xf, 0x2, 0x2, 0x2d, 0x32, 0x5, 0x4, 0x3, 0x2, 0x2e, 0x2f, 
       0x7, 0x11, 0x2, 0x2, 0x2f, 0x31, 0x5, 0x4, 0x3, 0x2, 0x30, 0x2e, 
       0x3, 0x2, 0x2, 0x2, 0x31, 0x34, 0x3, 0x2, 0x2, 0x2, 0x32, 0x30, 0x3, 
       0x2, 0x2, 0x2, 0x32, 0x33, 0x3, 0x2, 0x2, 0x2, 0x33, 0x36, 0x3, 0x2, 
       0x2, 0x2, 0x34, 0x32, 0x3, 0x2, 0x2, 0x2, 0x35, 0x37, 0x7, 0x11, 
       0x2, 0x2, 0x36, 0x35, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x3, 0x2, 0x2, 
       0x2, 0x37, 0x39, 0x3, 0x2, 0x2, 0x2, 0x38, 0x2d, 0x3, 0x2, 0x2, 0x2, 
       0x38, 0x39, 0x3, 0x2, 0x2, 0x2, 0x39, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3a, 
       0x63, 0x7, 0x10, 0x2, 0x2, 0x3b, 0x47, 0x7, 0xb, 0x2, 0x2, 0x3c, 
       0x41, 0x5, 0x4, 0x3, 0x2, 0x3d, 0x3e, 0x7, 0x11, 0x2, 0x2, 0x3e, 
       0x40, 0x5, 0x4, 0x3, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x40, 0x43, 
       0x3, 0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x3, 
       0x2, 0x2, 0x2, 0x42, 0x45, 0x3, 0x2, 0x2, 0x2, 0x43, 0x41, 0x3, 0x2, 
       0x2, 0x2, 0x44, 0x46, 0x7, 0x11, 0x2, 0x2, 0x45, 0x44, 0x3, 0x2, 
       0x2, 0x2, 0x45, 0x46, 0x3, 0x2, 0x2, 0x2, 0x46, 0x48, 0x3, 0x2, 0x2, 
       0x2, 0x47, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 0x2, 
       0x48, 0x49, 0x3, 0x2, 0x2, 0x2, 0x49, 0x63, 0x7, 0xc, 0x2, 0x2, 0x4a, 
       0x53, 0x7, 0xf, 0x2, 0x2, 0x4b, 0x50, 0x5, 0x4, 0x3, 0x2, 0x4c, 0x4d, 
       0x7, 0x11, 0x2, 0x2, 0x4d, 0x4f, 0x5, 0x4, 0x3, 0x2, 0x4e, 0x4c, 
       0x3, 0x2, 0x2, 0x2, 0x4f, 0x52, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4e, 0x3, 
       0x2, 0x2, 0x2, 0x50, 0x51, 0x3, 0x2, 0x2, 0x2, 0x51, 0x54, 0x3, 0x2, 
       0x2, 0x2, 0x52, 0x50, 0x3, 0x2, 0x2, 0x2, 0x53, 0x4b, 0x3, 0x2, 0x2, 
       0x2, 0x53, 0x54, 0x3, 0x2, 0x2, 0x2, 0x54, 0x55, 0x3, 0x2, 0x2, 0x2, 
       0x55, 0x63, 0x8, 0x4, 0x1, 0x2, 0x56, 0x5f, 0x7, 0xb, 0x2, 0x2, 0x57, 
       0x5c, 0x5, 0x4, 0x3, 0x2, 0x58, 0x59, 0x7, 0x11, 0x2, 0x2, 0x59, 
       0x5b, 0x5, 0x4, 0x3, 0x2, 0x5a, 0x58, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5e, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x3, 
       0x2, 0x2, 0x2, 0x5d, 0x60, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5c, 0x3, 0x2, 
       0x2, 0x2, 0x5f, 0x57, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x60, 0x3, 0x2, 0x2, 
       0x2, 0x60, 0x61, 0x3, 0x2, 0x2, 0x2, 0x61, 0x63, 0x8, 0x4, 0x1, 0x2, 
       0x62, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x62, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x62, 
       0x4a, 0x3, 0x2, 0x2, 0x2, 0x62, 0x56, 0x3, 0x2, 0x2, 0x2, 0x63, 0x7, 
       0x3, 0x2, 0x2, 0x2, 0x64, 0x66, 0x9, 0x2, 0x2, 0x2, 0x65, 0x64, 0x3, 
       0x2, 0x2, 0x2, 0x65, 0x66, 0x3, 0x2, 0x2, 0x2, 0x66, 0x67, 0x3, 0x2, 
       0x2, 0x2, 0x67, 0x69, 0x7, 0x15, 0x2, 0x2, 0x68, 0x6a, 0x5, 0x4, 
       0x3, 0x2, 0x69, 0x68, 0x3, 0x2, 0x2, 0x2, 0x69, 0x6a, 0x3, 0x2, 0x2, 
       0x2, 0x6a, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x7d, 0x7, 0x6, 0x2, 0x2, 
       0x6c, 0x7d, 0x7, 0x7, 0x2, 0x2, 0x6d, 0x7d, 0x7, 0x9, 0x2, 0x2, 0x6e, 
       0x7d, 0x7, 0xa, 0x2, 0x2, 0x6f, 0x71, 0x7, 0xd, 0x2, 0x2, 0x70, 0x72, 
       0x5, 0x4, 0x3, 0x2, 0x71, 0x70, 0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 0x3, 
       0x2, 0x2, 0x2, 0x72, 0x77, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x7, 0x11, 
       0x2, 0x2, 0x74, 0x76, 0x5, 0x4, 0x3, 0x2, 0x75, 0x73, 0x3, 0x2, 0x2, 
       0x2, 0x76, 0x79, 0x3, 0x2, 0x2, 0x2, 0x77, 0x75, 0x3, 0x2, 0x2, 0x2, 
       0x77, 0x78, 0x3, 0x2, 0x2, 0x2, 0x78, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x79, 
       0x77, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7d, 0x7, 0xe, 0x2, 0x2, 0x7b, 0x7d, 
       0x7, 0x13, 0x2, 0x2, 0x7c, 0x65, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x6b, 
       0x3, 0x2, 0x2, 0x2, 0x7c, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x6d, 0x3, 
       0x2, 0x2, 0x2, 0x7c, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x6f, 0x3, 0x2, 
       0x2, 0x2, 0x7c, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x9, 0x3, 0x2, 0x2, 
       0x2, 0x7e, 0x83, 0x7, 0x3, 0x2, 0x2, 0x7f, 0x83, 0x7, 0x5, 0x2, 0x2, 
       0x80, 0x83, 0x7, 0x4, 0x2, 0x2, 0x81, 0x83, 0x5, 0x6, 0x4, 0x2, 0x82, 
       0x7e, 0x3, 0x2, 0x2, 0x2, 0x82, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x82, 0x80, 
       0x3, 0x2, 0x2, 0x2, 0x82, 0x81, 0x3, 0x2, 0x2, 0x2, 0x83, 0xb, 0x3, 
       0x2, 0x2, 0x2, 0x84, 0x89, 0x7, 0x3, 0x2, 0x2, 0x85, 0x89, 0x7, 0x5, 
       0x2, 0x2, 0x86, 0x89, 0x7, 0x4, 0x2, 0x2, 0x87, 0x89, 0x5, 0x6, 0x4, 
       0x2, 0x88, 0x84, 0x3, 0x2, 0x2, 0x2, 0x88, 0x85, 0x3, 0x2, 0x2, 0x2, 
       0x88, 0x86, 0x3, 0x2, 0x2, 0x2, 0x88, 0x87, 0x3, 0x2, 0x2, 0x2, 0x89, 
       0xd, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x5, 0xc, 0x7, 0x2, 0x8b, 0x8c, 
       0x5, 0xa, 0x6, 0x2, 0x8c, 0x91, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 0x5, 
       0xc, 0x7, 0x2, 0x8e, 0x8f, 0x5, 0xe, 0x8, 0x2, 0x8f, 0x91, 0x3, 0x2, 
       0x2, 0x2, 0x90, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x90, 0x8d, 0x3, 0x2, 0x2, 
       0x2, 0x91, 0xf, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x15, 0x19, 0x20, 0x24, 
       0x2a, 0x32, 0x36, 0x38, 0x41, 0x45, 0x47, 0x50, 0x53, 0x5c, 0x5f, 
       0x62, 0x65, 0x69, 0x71, 0x77, 0x7c, 0x82, 0x88, 0x90, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

oscriptParser::Initializer oscriptParser::_init;
