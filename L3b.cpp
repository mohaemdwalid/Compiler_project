#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, FLOAT, STRING, CHAR,
    OPERATOR, SYMBOL, COMMENT, PREPROCESSOR, UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

void read_file(string filepath, string& buffer);

class Parser {
public:
    Parser(const vector<Token>& tokens) : tokens(tokens), position(0) {}

    void parse() {
        while (position < tokens.size()) {
            stmt_list();
        }
    }

private:
    vector<Token> tokens;
    size_t position;

    void consume() {
        if (position < tokens.size())
            position++;
    }

    const Token& peek() const {
        if (position < tokens.size())
            return tokens[position];
        else {
            static Token nullToken = {UNKNOWN, ""};
            return nullToken;
        }
    }

    void match(TokenType expected) {
        if (peek().type == expected)
            consume();
        else
            throw std::runtime_error("Unexpected token type");
    }

    void stmt_list() {
    while (position < tokens.size() && (peek().type != SYMBOL || peek().value != "}")) {
        stmt();
    }
    }

    

void stmt() {
    if (peek().type == SYMBOL && peek().value == ";") {
        cout << "Parsed a statement: Empty statement" << endl;
        match(SYMBOL);
    } else if (peek().type == IDENTIFIER) {
        cout << "Parsed a statement: Assignment statement" << endl;
        match(IDENTIFIER);
        if (peek().value == "=") {
            match(SYMBOL); // '='
            expr();
        }
        match(SYMBOL); // ';'
    } else if (peek().type == KEYWORD && peek().value == "while") {
        cout << "Parsed a statement: While loop statement" << endl;
        match(KEYWORD);
        paren_expr();
        stmt();
    } else if (peek().type == KEYWORD && peek().value == "if") {
        cout << "Parsed a statement: If statement" << endl;
        match(KEYWORD);
        paren_expr();
        stmt();
        if (peek().type == KEYWORD && peek().value == "else") {
            match(KEYWORD);
            stmt();
        }
    } else if (peek().type == KEYWORD && peek().value == "print") {
        cout << "Parsed a statement: Print statement" << endl;
        match(KEYWORD);
        match(SYMBOL); // '('
        prt_list();
        match(SYMBOL); // ')'
        match(SYMBOL); // ';'
    } else if (peek().type == KEYWORD && peek().value == "putc") {
        cout << "Parsed a statement: Putc statement" << endl;
        match(KEYWORD);
        paren_expr();
        match(SYMBOL); // ';'
    } else if (peek().type == SYMBOL && peek().value == "{") {
        cout << "Parsed a statement: Block statement" << endl;
        match(SYMBOL); // '{'
        stmt_list();
        match(SYMBOL); // '}'
    } else {
        throw std::runtime_error("Invalid statement");
    }
}



    void paren_expr() {
        match(SYMBOL); // '('
        expr();
        match(SYMBOL); // ')'
    }

    void prt_list() {
        if (peek().type == STRING || peek().type == INTEGER || peek().type == FLOAT || peek().type == IDENTIFIER) {
            match(peek().type);
            while (peek().type == SYMBOL && peek().value == ",") {
                match(SYMBOL); // ','
                if (peek().type == STRING || peek().type == INTEGER || peek().type == FLOAT || peek().type == IDENTIFIER) {
                    match(peek().type);
                } else {
                    throw std::runtime_error("Invalid expression in print list");
                }
            }
        }
    }

    void expr() {
        and_expr();
        while (peek().type == OPERATOR && (peek().value == "||")) {
            match(OPERATOR); // '||'
            and_expr();
        }
    }

    void and_expr() {
        equality_expr();
        while (peek().type == OPERATOR && (peek().value == "&&")) {
            match(OPERATOR); // '&&'
            equality_expr();
        }
    }

    void equality_expr() {
        relational_expr();
        if (peek().type == OPERATOR && (peek().value == "==" || peek().value == "!=")) {
            match(OPERATOR); // '==' or '!='
            relational_expr();
        }
    }

    void relational_expr() {
        addition_expr();
        if (peek().type == OPERATOR && (peek().value == "<" || peek().value == "<=" || peek().value == ">" || peek().value == ">=")) {
            match(OPERATOR); // '<', '<=', '>', or '>='
            addition_expr();
        }
    }

    void addition_expr() {
        multiplication_expr();
        while (peek().type == OPERATOR && (peek().value == "+" || peek().value == "-")) {
            match(OPERATOR); // '+' or '-'
            multiplication_expr();
        }
    }

    void multiplication_expr() {
        primary();
        while (peek().type == OPERATOR && (peek().value == "*" || peek().value == "/" || peek().value == "%")) {
            match(OPERATOR); // '*', '/', or '%'
            primary();
        }
    }

    void primary() {
        if (peek().type == IDENTIFIER || peek().type == INTEGER || peek().type == FLOAT || peek().type == STRING || peek().type == CHAR) {
            match(peek().type);
        } else if (peek().type == SYMBOL && peek().value == "(") {
            match(SYMBOL); // '('
            expr();
            match(SYMBOL); // ')'
        } else if (peek().type == OPERATOR && (peek().value == "+" || peek().value == "-" || peek().value == "!")) {
            match(OPERATOR); // '+', '-', or '!'
            primary();
        } else {
            throw std::runtime_error("Invalid primary expression");
        }
    }
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
    // string program;
    // read_file("C:\\Users\\youss\\OneDrive\\Desktop\\Semester 6\\Design of compilers\\compilers project\\Test.c", program);
    string program = R"(
      count = 1;
n = 1;
limit = 100;
while (n < limit) {
    k=3;
    p=1;
    n=n+2;
    while ((k*k<=n) && (p)) {
        p=n/k*k!=n;
        k=k+2;
    }
    if (p) {
        print(n, " is prime\n");
        count = count + 1;
    }
}
print("Total primes found: ", count, "\n");
      
    )";


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

    Parser parser(tokens);
    parser.parse();

    return 0;

}

void read_file(string filepath, string& buffer) {
    ifstream file(filepath);
    string line;
    while (getline(file, line)) {
        buffer += line + "\n";
    }
}
