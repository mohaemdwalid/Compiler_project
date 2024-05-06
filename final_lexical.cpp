#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void read_file(string filepath, string& buffer);

enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, FLOAT, STRING, CHAR,
    OPERATOR, SYMBOL, COMMENT, PREPROCESSOR, UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

const regex keywordRegex("\\b(auto|double|int|struct|break|else|long|switch|"
                              "case|enum|register|typedef|char|extern|return|union|"
                              "const|float|short|unsigned|continue|for|signed|void|"
                              "default|goto|sizeof|volatile|do|if|static|while)\\b");
const regex identifierRegex("[a-zA-Z_][a-zA-Z0-9_]*");
const regex integerRegex("\\b[0-9]+\\b");
const regex floatRegex("\\b[0-9]+\\.[0-9]+\\b");
const regex stringRegex("\"[^\"]*\"");
const regex charRegex("'[a-zA-Z]'");
const regex operatorRegex("\\+|\\-|\\*|\\/|%|<|>|==|!=|=|<=|>=");
const regex separatorRegex(";|\\(|\\)|\\{|\\}|,|'");
const regex preprocessorRegex("#[ \t]*(include|define|undef|if|ifdef|ifndef|else|elif|endif|line|error|pragma)");

TokenType getTokenType(const string& str) {
    if (regex_match(str, keywordRegex)) {
        return KEYWORD;
    } else if (regex_match(str, identifierRegex)) {
        return IDENTIFIER;
    } else if (regex_match(str, floatRegex)) {
        return FLOAT;
    } else if (regex_match(str, integerRegex)) {
        return INTEGER;
    } else if (regex_match(str, stringRegex)) {
        return STRING;
    } else if (regex_match(str, charRegex)) {
        return CHAR;
    } else if (regex_match(str, operatorRegex)) {
        return OPERATOR;
    } else if (regex_match(str, separatorRegex)) {
        return SYMBOL;
    } else if (regex_match(str, preprocessorRegex)) {
        return PREPROCESSOR;
    } else {
        return UNKNOWN;
    }
}

vector<Token> tokenize(const string& program, vector<string>& symbolTable) {
    vector<Token> tokens;

    regex tokenRegex("\\b(auto|double|int|struct|break|else|long|switch|"
                          "case|enum|register|typedef|char|extern|return|union|"
                          "const|float|short|unsigned|continue|for|signed|void|"
                          "default|goto|sizeof|volatile|do|if|static|while)\\b|"
                          "[a-zA-Z_][a-zA-Z0-9_]*|\\b[0-9]+\\.[0-9]+\\b|\\b[0-9]+\\b|"
                          "\"[^\"]*\"|'[a-zA-Z_]'|\\+|\\-|\\*|\\/|%|<|>|==|!=|=|<=|>="
                          "|;|\\(|\\)|\\{|\\}|,|"
                          "#[ \t]*(include|define|undef|if|ifdef|ifndef|else|elif|endif|line|error|pragma)");
    
    auto tokenBegin = sregex_iterator(program.begin(), program.end(), tokenRegex);
    auto tokenEnd = sregex_iterator();

    for (auto it = tokenBegin; it != tokenEnd; ++it) {
        string tokenValue = it->str();
        TokenType tokenType = getTokenType(tokenValue);
        tokens.push_back({tokenType, tokenValue});

        if (tokenType == IDENTIFIER) {
            bool found = false;
            for (const auto& entry : symbolTable) {
                if (entry == tokenValue) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                symbolTable.push_back(tokenValue);
            }
        }
    }

    return tokens;
}

void printSymbolTable(const vector<string>& symbolTable) {
    cout << "Symbol Table:" << endl;
    for (size_t i = 0; i < symbolTable.size(); ++i) {
        cout << "Index " << i << ": " << symbolTable[i] << endl;
    }
}

int main() {
    // Testing the tokenizer
    string program;
    read_file("C:\\Users\\youss\\OneDrive\\Desktop\\Semester 6\\Design of compilers\\compilers project\\Test.c", program);

    vector<string> symbolTable;
    vector<Token> tokens = tokenize(program, symbolTable);

    for (const auto& token : tokens) {
        switch (token.type) {
            case KEYWORD:
               cout << "KEYWORD: " << token.value << endl;
                break;
            case IDENTIFIER:
                cout << "IDENTIFIER: " << token.value << endl;
                break;
            case INTEGER:
                cout << "INTEGER: " << token.value << endl;
                break;
            case FLOAT:
                cout << "FLOAT: " << token.value << endl;
                break;
            case STRING:
                cout << "STRING: " << token.value << endl;
                break;
            case CHAR:
                cout << "CHAR: " << token.value << endl;
                break;
            case OPERATOR:
                cout << "OPERATOR: " << token.value << endl;
                break;
            case SYMBOL:
                cout << "SYMBOL: " << token.value << endl;
                break;
            case COMMENT:
                cout << "COMMENT: " << token.value << endl;
                break;
            case PREPROCESSOR:
                cout << "PREPROCESSOR: " << token.value << endl;
                break;
            default:
                cout << "UNKNOWN: " << token.value << endl;
                break;
        }
    }

    printSymbolTable(symbolTable);

    return 0;
}

void read_file(string filepath, string& buffer) {
    ifstream file(filepath);
    string line;
    while (getline(file, line)) {
        buffer += line + "\n";
    }
}

           
