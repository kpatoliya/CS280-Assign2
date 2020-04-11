/*
 * karan
 * CS280
 * Fall 2019
 */
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "lex.h"
using namespace std;

int main(int argc, char *argv[]){
    
    istream *in = &cin;
    ifstream file;
    
    Lex tok;
    int linenum = 0;
    
    bool vFlag = false;
    bool constsFlag = false;
    bool idsFlag = false;
    
    vector<string> identifiers;
    vector<string> strings;
    vector<int> integers;
    bool temp = true;
    
    int tokCount = 0;
    
    for(int i=1; i<argc; i++){
        string arg(argv[i]);
        if(arg == "-v")            
            vFlag = true;
        else if(arg == "-consts")
            constsFlag = true;
        else if(arg == "-ids")
            idsFlag = true;
        else if(arg[0] == '-'){
            cerr << "UNRECOGNIZED FLAG " << arg << endl;
            return 0;
        }
        else if(in != &cin){
            cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
            return 0;
        }
        else{ 
            file.open(arg);
            if( !file.is_open() ) {
                cerr << "CANNOT OPEN " << arg << endl;
                return 0;
            }
            in = &file;    
        }
    }
    
    while( (tok=getNextToken(*in, linenum) ) != DONE && tok != ERR ){
    
        if( vFlag )
            cout << tok << endl;
        ++tokCount;
        if( idsFlag ){   
            if( tok == ID ){
                if( !(find(identifiers.begin(), identifiers.end(), tok.GetLexeme()) != identifiers.end()) ) 
                    identifiers.push_back(tok.GetLexeme());    
            }
            
        }        
        if( constsFlag ){
            if( tok == STR )
                strings.push_back(tok.GetLexeme());
            else if( tok == INT ){                
                integers.push_back( stoi(tok.GetLexeme()) );      
            }
        }
                
    }
    
    if( tok == ERR ){
        cout << "Error on line " << linenum << " (" << tok.GetLexeme() << ")" << endl;
        return 0;
    }
    
    if( idsFlag ){
        sort( identifiers.begin(), identifiers.end() );
        temp = true;
        for( vector<string>::iterator iit = identifiers.begin(); iit != identifiers.end(); ++iit ){
            if( temp ){
                cout << "IDENTIFIERS: " << *iit;
                temp = false;
            }
            else{
                cout << ", ";
                cout << *iit;
            }
        }
        if( !identifiers.empty() )
            cout << endl;
    }
    
    if( constsFlag ){
        sort( strings.begin(), strings.end() );
        temp = true;
        for( vector<string>::iterator sit = strings.begin(); sit != strings.end(); ++sit ){
            if( temp ){
                cout << "STRINGS:" << endl << *sit << endl;
                temp = false;
            }
            else{
                cout << *sit;
                cout << endl;
            }
        }
        if( strings.empty() ){
            sort( integers.begin(), integers.end() );
            temp = true;
            for( vector<int>::iterator init = integers.begin(); init != integers.end(); ++init ){
                if( temp ){
                    cout << "INTEGERS:" << endl << *init ;
                    temp = false;
                }
                else{                    
                    cout << endl;
                    cout << *init;
                }
            }
            if( !integers.empty() )
                cout << endl;
        }        
    }    
        
    if( linenum == 0 ){
        cout << "Lines: " << linenum << endl;
    }
    else{
        cout << "Lines: " << linenum << endl;
        cout << "Tokens: " << tokCount << endl;
    }
    
    return 0;
}
    