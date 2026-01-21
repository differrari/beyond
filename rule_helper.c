#include "rules.h"

char *tok_symbol(TokenKind kind){
    switch (kind) {
        case TOK_LPAREN: return "(";
        case TOK_RPAREN: return ")";
        case TOK_LBRACE: return "{";
        case TOK_RBRACE: return "}";
        case TOK_LBRACKET: return "[";
        case TOK_RBRACKET: return "]";
        case TOK_COMMA: return ",";
        case TOK_COLON: return ":\n";
        case TOK_SEMICOLON: return ";\n";
        case TOK_DOT: return ".";
        case TOK_NEWLINE: return ";\n";
      default: return "";
    }
}