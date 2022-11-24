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
#include "sys/ose_load.h"

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

static void oscript_parseExpr(ose_bundle bundle)
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

static void oscript_parse(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    if(ose_bundleIsEmpty(vm_s))
    {
        return;
    }
    oscript_parseExpr(vm_s);
    ose_nip(vm_s);
}

static void oscript_load(ose_bundle osevm)
{
    ose_bundle vm_s = OSEVM_STACK(osevm);
    ose_bundle vm_c = OSEVM_CONTROL(osevm);
    ose_bundle vm_i = OSEVM_INPUT(osevm);
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
    if(!ose_bundleHasAtLeastNElems(vm_s, 1))
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_COUNT);
        return;
    }
    if(ose_peekType(vm_s) != OSETT_MESSAGE)
    {
        ose_errno_set(vm_s, OSE_ERR_ELEM_TYPE);
        return;
    }
    if(ose_peekMessageArgType(vm_s) != OSETT_STRING)
    {
        ose_errno_set(vm_s, OSE_ERR_ITEM_TYPE);
        return;
    }
    const char * const str = ose_peekString(vm_s);
    ose_readFile(vm_s, str);
    ose_nip(vm_s);
    ose_blobToType_impl(vm_s, OSETT_STRING);
    // oscript_parse(vm_s);
    /* ose_exec1(vm_s); */
    /* ose_drop(vm_s); */

    ose_pushString(vm_c, "/!/drop");
    ose_swap(vm_c);
    ose_pushString(vm_c, "/!/exec1");
    ose_swap(vm_c);
    ose_pushString(vm_c, "/!/o/parse");
    ose_swap(vm_c);

    ose_2drop(vm_i);
    ose_pushString(vm_i, "/!/o/finalize/toplevelexec");
}

void oscript_parser_load(ose_bundle vm_s)
{
    ose_pushMessage(vm_s, "/o/parse",
                    strlen("/o/parse"),
                    1, OSETT_ALIGNEDPTR, oscript_parse);
    ose_pushMessage(vm_s, "/o/load",
                    strlen("/o/load"),
                    1, OSETT_ALIGNEDPTR, oscript_load);
}

#ifdef __cplusplus
}
#endif
