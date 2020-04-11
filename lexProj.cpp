/*
 * karan
 * CS280
 * Fall 2019
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "lex.h"
using namespace std;

static map<Token,string> printToken = {
    { PRINT, "PRINT" },
    { LET, "LET" },
    { IF, "IF" },
    { LOOP, "LOOP" },
    { BEGIN, "BEGIN" },
    { END, "END" },    
    
    { ID, "ID" },
    
    { INT, "INT" },
    { STR, "STR" },
    
    
    { PLUS, "PLUS" },
    { MINUS, "MINUS" },
    { STAR, "STAR" },
    { SLASH, "SLASH"},
    { BANG, "BANG"},
    { LPAREN, "LPAREN" },
    { RPAREN, "RPAREN" },
    { SC, "SC" },

    { ERR, "ERR" },

    { DONE, "DONE" }
};


ostream& operator<<(ostream& out, const Lex& tok) {
    Token tt = tok.GetToken();
    out << printToken[ tt ];
    if( tt == ID || tt == INT || tt == STR || tt == ERR ) {
        out  << "(" << tok.GetLexeme() << ")";
    }
    
    return out;
}


static map<string,Token> kwmap = {
        { "print", PRINT },
        { "let", LET },
        { "if", IF },
        { "loop", LOOP },
        { "begin", BEGIN },
        { "end", END },
};

Lex id_or_kw(const string& lexeme, int linenum){
    Token tt = ID;

    auto kIt = kwmap.find(lexeme);
    if( kIt != kwmap.end() )
        tt = kIt->second;

    return Lex(tt, lexeme, linenum);
}

Lex getNextToken(istream& in, int& linenum){
    enum LexState { BEGIN, INID, INSTR, ININT, INCOM } lexstate = BEGIN;
    string lexeme;
    char ch;
    while( in.get(ch) ){
        
        if( ch == '\n' ){
            linenum++;
        }        
        switch( lexstate ){
            case BEGIN:
                if( isspace(ch) )
                    continue;

                lexeme = ch;

                if( isalpha(ch) ){
                    lexstate = INID;
                }
                else if( ch == '"' ){
                    lexstate = INSTR;
                }                
                else if( isdigit(ch) ){
                    lexstate = ININT;
                }
                else if( ch == '/'){
                    in.get(ch);
                    if( ch == '/' ) {
                        lexstate = INCOM;
                    }else{
                        in.putback(ch);
                        return Lex(SLASH, lexeme, linenum);
                    }
                }
                else{
                    Token tt = ERR;
                    switch( ch ){
                        case '+':
                            tt = PLUS;
                            break;
                        case '-':
                            tt = MINUS;
                            break;
                        case '*':
                            tt = STAR;
                            break;
                        case '/':
                            tt = SLASH;
                            break;
                        case '!':
                            tt = BANG;
                            break;
                        case '(':
                            tt = LPAREN;
                            break;
                        case ')':
                            tt = RPAREN;
                            break;
                        case ';':
                            tt = SC;
                            break;
                    }
                   if( tt == ERR ){
                       linenum++;
                       return Lex(tt, lexeme, linenum);
                   }
                    else{
                        return Lex(tt, lexeme, linenum);
                    }
                }
                break;
                
            case INID:
                if( isalpha(ch) || isdigit(ch) ){
                    lexeme += ch;
                }
                else{
                    if( ch == '\n' )
                        linenum--;
                    in.putback(ch);
                    
                    return id_or_kw(lexeme, linenum);
                }                
                break;
                
            case INSTR: 
                lexeme += ch;
                if( ch == '\n' ){
                    return Lex(ERR, lexeme, linenum);
                }
                if( ch == '\\' ){
                   lexeme = lexeme.substr(0, lexeme.length()-1); 
                   in.get(ch);
                   if( ch == '\\' ){
                      lexeme += ch;
                      continue;
                   }
                   else if( isalpha(ch) || ispunct(ch) ){
                       if(ch == 'n'){
                           lexeme += "\n";                          
                       }
                       else{
                           lexeme += ch;
                       }
                   }
                   else if ( isspace(ch) ){
                       lexeme += ch;                                             
                   } 
                   break;
                }
                if( ch == '"' ){
                    lexeme = lexeme.substr(1, lexeme.length()-2);
                    return Lex(STR, lexeme, linenum);                    
                }                
                break;
            
            case ININT:
                if( isdigit(ch) ){
                    lexeme += ch;
                }
                else if( isalpha(ch) ){
                    lexeme += ch;
                    return Lex(ERR, lexeme, linenum);
                }
                else{
                    if( ch == '\n' )
                        (linenum)--;
                    in.putback(ch);
                    return Lex(INT, lexeme, linenum);
                }
                break;
                
            case INCOM:
                if( ch == '\n' ){
                    lexstate = BEGIN;
                }
                break;                    
        }       
    } 
    return Lex(DONE, "", linenum);
}
