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

#include "antlr4-runtime.h"
#include "oscriptLexer.h"
#include "oscriptParser.h"
#include "oscriptVisitorImpl.h"

using namespace std;
using namespace antlr4;
using namespace antlrcpp;
using namespace oscript;

class ParserErrorListener : public BaseErrorListener {
private:
    oscriptVisitorImpl *visitor = NULL;
    oscriptParser *parser = NULL;
    oscriptLexer *lexer = NULL;
public:
    ParserErrorListener(oscriptVisitorImpl *_visitor,
                        oscriptParser *_parser,
                        oscriptLexer *_lexer)
        : visitor(_visitor), parser(_parser), lexer(_lexer){}
    void syntaxError(Recognizer *recognizer,
                     Token *offendingSymbol,
                     size_t line,
                     size_t charPositionInLine,
                     const std::string &msg,
                     std::exception_ptr e)
    {
        // silence compiler warnings about unused vars
        (void)visitor, (void)parser, (void)lexer;
        cerr << "Parse error " << line << ":" <<
            charPositionInLine << " " << msg << "\n";
        if(offendingSymbol)
        {
            std::string symstr = offendingSymbol->getText();
            int symtype = offendingSymbol->getType();
            std::string symtypestr = parser->getVocabulary()
                .getSymbolicName(symtype);
            cerr << "Token \"";
            cerr << symstr;
            cerr << "\" was classified as: ";
            cerr << symtypestr;
            cerr << "\n";
        }
    }
};

class LexerErrorListener : public BaseErrorListener {
private:
    oscriptVisitorImpl *visitor = NULL;
    oscriptParser *parser = NULL;
    oscriptLexer *lexer = NULL;
public:
    LexerErrorListener(oscriptVisitorImpl *_visitor,
                       oscriptParser *_parser,
                       oscriptLexer *_lexer)
        : visitor(_visitor), parser(_parser), lexer(_lexer){}
    void syntaxError(Recognizer *recognizer,
                     Token *offendingSymbol,
                     size_t line,
                     size_t charPositionInLine,
                     const std::string &msg,
                     std::exception_ptr e)
    {
        // silence compiler warnings about unused vars
        (void)visitor, (void)parser, (void)lexer;
        // visitor->incNErrors();
        cerr << "Parse error " << line << ":" <<
            charPositionInLine << " " << msg << "\n";
        if(offendingSymbol)
        {
            cerr << "Symbol: " << offendingSymbol->toString() << "\n";
        }
    }
};

#ifdef __cplusplus
extern "C" {
#endif

static void oscript_parse_impl(const char * const str,
                               const int32_t len,
                               ose_bundle bundle)
{
    ANTLRInputStream input(str, len);
    oscriptLexer lexer(&input);
    
    CommonTokenStream tokens(&lexer);
    
    oscriptVisitorImpl *visitor = new oscriptVisitorImpl(bundle);

    oscriptParser parser(&tokens);

    LexerErrorListener lexerErrorListener(visitor, &parser, &lexer);
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrorListener);
    
    ParserErrorListener parserErrorListener(visitor, &parser, &lexer);
    parser.removeParseListeners();
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrorListener);

    oscriptParser::TopLevelBundleContext *tree =
        parser.topLevelBundle();
    Any a = visitor->visitTopLevelBundle(tree);
}

void oscript_parseExpr(ose_bundle bundle)
{
    ose_assert(ose_bundleHasAtLeastNElems(bundle, 1));
    ose_assert(ose_peekType(bundle) == OSETT_MESSAGE);
    ose_assert(ose_isStringType(ose_peekMessageArgType(bundle)));
    const char * const str = ose_peekString(bundle);
    const int32_t len = strlen(str);
    if(len == 0)
    {
        ose_pushBundle(bundle);
    }
    else
    {
        oscript_parse_impl(str, len, bundle);
    }
}

void oscript_parse(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(ose_bundleIsEmpty(vm_s))
    {
        return;
    }
    oscript_parseExpr(vm_s);
    ose_nip(vm_s);
}

void oscript_parser_load(ose_bundle vm_s)
{
    ose_pushMessage(vm_s, "/o/parse",
                    strlen("/o/parse"),
                    1, OSETT_ALIGNEDPTR, oscript_parse);
}

#ifdef __cplusplus
}
#endif
