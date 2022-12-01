grammar oscript;

topLevelBundle
    : first=oscBundleElem (SeqSep rest=oscBundleElem)* SeqSep? EOF
    | CommentLine* EOF
    ;

oscBundleElem
    : (Identifier | OSCAddress | OSCPattern) # LookupElem
    | funcall # FuncallElem
    | oscBundle # BundleElem
    | oscMessage # MessageElem
    //| args=oscBundleElem fn=oscBundleElem # FuncallElem
    ;

oscBundle
    : LBrace
        (first=oscBundleElem
            (SeqSep rest=oscBundleElem)* SeqSep?)?
        RBrace # lazyBundle
    | LParen
        (first=oscBundleElem
            (SeqSep rest=oscBundleElem)* SeqSep?)?
        RParen # eagerBundle
    // error: unclosed paren/brace
    | LBrace
        (first=oscBundleElem (SeqSep rest=oscBundleElem)*)?
        {
            notifyErrorListeners(_input->LT(-1),
                                 "Unclosed bundle: missing '}'",
                                 NULL);
        } # unclosedLazyBundle
    | LParen
        (first=oscBundleElem (SeqSep rest=oscBundleElem)*)?
        {
            notifyErrorListeners(_input->LT(-1),
                                 "Unclosed bundle: missing ')'",
                                 NULL);
        } # unclosedEagerBundle
    ;

oscMessage
    : (Identifier | OSCAddress | OSCPattern)? Bind oscBundleElem? # bind
    | Int32 # int
    | Float # float
    | String # string
    | Blob # blob
    | LBracket first=oscBundleElem?
        (SeqSep rest=oscBundleElem)* RBracket # list
    | CommentMessage # comment
    ;

fn
    : Identifier
    | OSCAddress
    | OSCPattern
    | oscBundle
    ;

args
    : Identifier
    | OSCAddress
    | OSCPattern
    | oscBundle
    ;

funcall
    : args fn
    | args funcall
    ;

Identifier
    : [_a-zA-Z][_0-9a-zA-Z]*
    // | ('/' [_a-zA-Z0-9]+)+
    ;

OSCPattern
    : (Container | PContainer)* PContainer+ (Container | PContainer)*
    ;

fragment
PContainer
    : '/' (ValidAddressChar | Wildcard)* Wildcard+ (ValidAddressChar | Wildcard)*
    ;

fragment
Wildcard
    : '?'
    | '*'
    | '{' ~[ {}]+ (SeqSep ~[ {}]+)* '}'
    | '[' '^'? ~[\]]+ ']'
    ;

OSCAddress
    : Container+
    ;

fragment
Container
    : '/' ValidAddressChar*
    ;

fragment
ValidAddressChar
// This is better expressed as this negation, but the first
// square bracket inside the char class causes antlr-mode's
// indentation to screw up, and it can't be escaped as antlr4
// produces a warning.
//: ~[ #*,/?[\](){}\t\n:]
    : [!"$%&'+\-.0-9;<=>@A-Z\\^_`a-z|~]
    ;

Int32
    : IntegerConstant
    ;

Float
    : FloatingConstant
    ;

fragment
IntegerConstant
    :   DecimalConstant // IntegerSuffix?
    |   OctalConstant // IntegerSuffix?
    |   HexadecimalConstant // IntegerSuffix?
    |   BinaryConstant
    ;

fragment
BinaryConstant
    :    '0' [bB] [0-1]+
    ;

fragment
DecimalConstant
    :   NonzeroDigit Digit*
    ;

fragment
OctalConstant
    :   '0' OctalDigit*
    ;

fragment
HexadecimalConstant
    :   HexadecimalPrefix HexadecimalDigit+
    ;

fragment
HexadecimalPrefix
    :   '0' [xX]
    ;

fragment
NonzeroDigit
    :   [1-9]
    ;

fragment
OctalDigit
    :   [0-7]
    ;

fragment
HexadecimalDigit
    :   [0-9a-fA-F]
    ;

fragment
FloatingConstant
    :   DecimalFloatingConstant
    |   HexadecimalFloatingConstant
    ;

fragment
DecimalFloatingConstant
    :   FractionalConstant ExponentPart? FloatingSuffix?
    |   DigitSequence ExponentPart FloatingSuffix?
    ;

fragment
HexadecimalFloatingConstant
    :   HexadecimalPrefix HexadecimalFractionalConstant BinaryExponentPart FloatingSuffix?
    |   HexadecimalPrefix HexadecimalDigitSequence BinaryExponentPart FloatingSuffix?
    ;

fragment
FractionalConstant
    :   DigitSequence? '.' DigitSequence
    |   DigitSequence '.'
    ;

fragment
ExponentPart
    :   'e' Sign? DigitSequence
    |   'E' Sign? DigitSequence
    ;

fragment
Sign
    :   '+' | '-'
    ;

DigitSequence
    :   Digit+
    ;

fragment
HexadecimalFractionalConstant
    :   HexadecimalDigitSequence? '.' HexadecimalDigitSequence
    |   HexadecimalDigitSequence '.'
    ;

fragment
BinaryExponentPart
    :   'p' Sign? DigitSequence
    |   'P' Sign? DigitSequence
    ;

fragment
HexadecimalDigitSequence
    :   HexadecimalDigit+
    ;

fragment
FloatingSuffix
    :   'f' | 'l' | 'F' | 'L'
    ;

fragment
Digit
    :   [0-9]
    ;

String
    : '"' (~[\\"] | '\\' [\\"])* '"'
    ;        

Blob
    : 'b' (Digit Digit)*
    ;

LParen          : '(' ;
RParen          : ')' ;
LBracket        : '[' ;
RBracket        : ']' ;
LBrace          : '{' ;
RBrace          : '}' ;
// LDoubleBracket  : '[[';
// RDoubleBracket  : ']]';

SeqSep          : ',' ;

CommentLine     :  '#' ~( '\r' | '\n' )* -> skip ;
CommentMessage : '/#/' ~( '\r' | '\n' | ',' )* ;
ClearSpace  : [ \t\n\r]+ -> skip ;

// Add                 : '+' ;
// Sub                 : '-' ;
// Mul                 : '*' ;
// Div                 : '/' ;
// Pow                 : '^' ;
// Mod                 : '%' ;
// Not                 : '!' ;
// Dot                 : '.' ;
// QuestionMark        : '?' ;
// DoubleQuestionMark  : '??' ;

// LessThan            : '<' ;
// GreaterThan         : '>' ;
// LessThanOrEqual     : '<=' ;
// GreaterThanOrEqual  : '>=' ;
// Equal               : '==' ;
// NotEqual            : '!=' ;

Bind                : ':' ;

// Assign 
//     : '='
//     | '+='
//     | '-='
//     | '*='
//     | '/='
//     | '^='
//     | '%='
//     | '??='
//     ;

