
// Generated from oscript.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"


namespace oscript {


class  oscriptLexer : public antlr4::Lexer {
public:
  enum {
    Identifier = 1, OSCPattern = 2, OSCAddress = 3, Int32 = 4, Float = 5, 
    DigitSequence = 6, String = 7, Blob = 8, LParen = 9, RParen = 10, LBracket = 11, 
    RBracket = 12, LBrace = 13, RBrace = 14, SeqSep = 15, CommentLine = 16, 
    CommentMessage = 17, ClearSpace = 18, Bind = 19
  };

  explicit oscriptLexer(antlr4::CharStream *input);
  ~oscriptLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace oscript
