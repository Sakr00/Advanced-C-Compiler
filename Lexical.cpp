#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <vector>

using namespace std;

struct tokens{
    string name, type;
};

bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == '=')
		return true;
	return false;
}

bool isConditionalOperator(vector<char>::iterator ch, int *len)
{
    if(*ch == '=' && *(ch+1) == '=')
    {
        *len = 2;
        return true;
    }
    else if(*ch == '<' && *(ch+1) == '=')
    {
        *len = 2;
        return true;
    }
    else if(*ch == '>' && *(ch+1) == '=')
    {
        *len = 2;
        return true;
    }
    else if (*ch == '>' || *ch == '<')
    {
        *len = 1;
        return true;
    }
    *len = 0;
	return false;
}

bool isDelimiter(char ch)
{
	if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
		ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
		ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
		ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"'
		|| ch == '\'' || ch == '%' || ch =='\\' || ch=='#') return true;
	return false;
}

bool validIdentifier(string str)
{
	if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
		str[0] == '3' || str[0] == '4' || str[0] == '5' ||
		str[0] == '6' || str[0] == '7' || str[0] == '8' ||
		str[0] == '9' || isDelimiter(str[0]) == true) return false;
	return true;
}

bool isKeyword(string str)
{
    if (!str.compare("if") || !str.compare("else") ||
        !str.compare("while") || !str.compare("do") ||
        !str.compare("break") || !str.compare("continue")
        || !str.compare("int")|| !str.compare("printf")
        || !str.compare("double") || !str.compare("float")
        || !str.compare("return") || !str.compare("char")
        || !str.compare("case") || !str.compare("define")
        || !str.compare("sizeof") || !str.compare("long")
        || !str.compare("short") || !str.compare("typedef")
        || !str.compare("switch") || !str.compare("unsigned")
        || !str.compare("void") || !str.compare("static")
        || !str.compare("struct") || !str.compare("goto")
        || !str.compare("include") || !str.compare("string"))
        return true;
    return false;
}


bool isInteger(string str)
{
    int i, len = str.length();

    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' || (str[i] == '-' && i > 0))
            return false;
    }
    return true;
}

bool isRealNumber(string str)
{
    int i, len = str.length();
    bool hasDecimal = false;

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.' ||
            (str[i] == '-' && i > 0))
            return (false);
        if (str[i] == '.')
            hasDecimal = true;
    }
    return (hasDecimal);
}


bool is_char(char chr){
    if((chr >= 'a' && chr <='z') || (chr >= 'A' && chr <='Z'))
        return true;
    return false;
}

string vector_substring(vector<char> vec, vector<char>::iterator first, vector<char>::iterator second)
{
    string tmp;
    while(first < second)
    {
        tmp += *first;
        first++;
    }
    return tmp;
}

bool is_placeHolder(string placeholder)
{
    if(placeholder == "%d" || placeholder == "%f") return true;
    return false;
}

bool is_escapeChar(string placeholder)
{
    if(placeholder == "\\n" || placeholder == "\\t") return true;
    return false;
}

string charToString(char chr)
{
    string s(1, chr);
    return s;
}

string upper(string str)
{
    for(int i = 0; i < str.length(); i++)
        str[i] = toupper(str[i]);
    return str;
}

void parser(string str, vector<tokens> *final_output_tokens)
{
    vector<tokens> output_tokens;
    vector<char> code;
    for (int i = 0; i < str.length(); i++)
        code.push_back(str[i]);

    vector<char>::iterator first = code.begin(), second = code.begin();
    while(second < code.end() && first <= second)
    {
        if(*second == '/' && *(second+1) == '/')
        {
            output_tokens.push_back({"//", "START_ONE_LINE_COMMENT_KEYWORD"});
            second+=2;
            string comment;
            while(second != code.end())
            {
                comment += *second;
                second++;
            }
            output_tokens.push_back({comment, "COMMENT"});
            break;
        }
        else if(*second == '/' && *(second+1) == '*')
        {
            output_tokens.push_back({"/*", "START_MULTIPLE_COMMENT_KEYWORD"});
            second += 2;
            string comment;
            while(*second != '*' && *(second+1) != '/')
            {
                comment += *second;
                second++;
            }
            output_tokens.push_back({comment, "COMMENT"});
            output_tokens.push_back({"*/", "END_MULTIPLE_COMMENT_KEYWORD"});
            second++;
            if(second == code.end()) return;
            second++;
            first = second;
        }
        if(!isDelimiter(*second)) second++;
        if (isDelimiter(*second) == true && first == second) {
            string tmp;
            if(second+1 < code.end())
            {
                tmp += *second;
                tmp += *(second + 1);
            }
            int len;
            if(second+1 != code.end() && isConditionalOperator(second, &len) == true)
            {
                if(len == 1)
                    output_tokens.push_back({charToString(*second), "CONDITION_OPERATOR"});
                else if(len == 2)
                {
                    output_tokens.push_back({charToString(*second) + charToString(*(second+1)), "CONDITION_OPERATOR"});
                    second++;
                }
            }
            else if (isOperator(*second) == true)
            {
                if(*second == '=')
                    output_tokens.push_back({charToString(*second), "EQ_OPERATOR"});
                else
                    output_tokens.push_back({charToString(*second), "OPERATOR"});
            }
            else if(*second == '(')
                output_tokens.push_back({charToString(*second), "OPEN_PAREN_SPECIAL_SYMBOL"});
            else if(*second == ')')
                output_tokens.push_back({charToString(*second), "CLOSE_PAREN_SPECIAL_SYMBOL"});
            else if(*second == '{')
                output_tokens.push_back({charToString(*second), "OPEN_BRACE_SPECIAL_SYMBOL"});
            else if(*second == '}')
                output_tokens.push_back({charToString(*second), "CLOSE_BRACE_SPECIAL_SYMBOL"});
            else if(*second == ',')
                output_tokens.push_back({charToString(*second), "COMMA_SPECIAL_SYMBOL"});
            else if(*second == ';')
                output_tokens.push_back({charToString(*second), "SEMICOLON_SPECIAL_SYMBOL"});
            else if(*second == '"')
                output_tokens.push_back({charToString(*second), "QOUTATION_SPECIAL_SYMBOL"});
            else if(*second == '#')
                output_tokens.push_back({charToString(*second), "PREPROCESSOR_DIRECTIVE"});
            else if (is_placeHolder(tmp))
            {
                output_tokens.push_back({tmp, "PLACE_HOLDER_KEYWORD"});
                second++;
                first = second;
            }
            else if(is_escapeChar(tmp))
            {
                output_tokens.push_back({tmp, "ESC_SEQ_KEYWORD"});
                second++;
                first = second;
            }
            else if(*second != ' ')
                output_tokens.push_back({charToString(*second), "SPECIAL_SYMBOL"});
            first++;
            second++;
        }
        else if (isDelimiter(*second) == true && first != second
                   || (second == code.end() && first  != second)) {
            string subStr = vector_substring(code, first, second);

            if (isKeyword(subStr) == true)
                output_tokens.push_back({subStr, upper(subStr) + "_KEYWORD"});
            else if (isInteger(subStr) == true)
                output_tokens.push_back({subStr, "NUM_NUMBER"});
            else if (isRealNumber(subStr) == true)
                output_tokens.push_back({subStr, "FLOAT_KEYWORD"});
            else if (validIdentifier(subStr) == true
                     && isDelimiter(*(second-1)) == false)
                output_tokens.push_back({subStr, "VAR_IDENTIFIER"});
            else if (validIdentifier(subStr) == false
                     && isDelimiter(*(second-1)) == false)
                output_tokens.push_back({subStr, "NOT_VAR_IDENTIFIER"});
            first = second;
        }
    }
    (*final_output_tokens).insert((*final_output_tokens).end(), output_tokens.begin(), output_tokens.end());
}

void printFile(string fileName, vector<tokens> out_tokens)
{
    ofstream fout;
    fout.open(fileName);
    for (int i = 0; i < out_tokens.size(); i++)
    fout << out_tokens[i].name << "\t--> " << out_tokens[i].type << endl;
    fout.close();
}

int main()
{
    vector<tokens> out_tokens;
    string myText, res;
    ifstream MyReadFile("while_loop.txt");
    if(!MyReadFile.is_open()){
        cout<<"Error while opening the file\n";
        exit(0);
    }
    while(getline(MyReadFile, myText))
        parser(myText, &out_tokens);
    MyReadFile.close();
    for (int i = 0; i < out_tokens.size(); i++)
        cout << out_tokens[i].name << "\t--> " << out_tokens[i].type << endl;

    printFile("output_parser.txt", out_tokens);
    return 0;
}
