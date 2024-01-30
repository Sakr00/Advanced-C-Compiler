#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
struct Data_Type_Variables{
    string type;
    string variable;

};

  int one_more_step_back_qoutation= 0;
  int index_Data_Type_Variables = 0;
  vector<Data_Type_Variables>declared_variables  ={{"empty","empty"}};//to check the data_types



  int syntax_semantic=0;


struct Node {
    string label;
    vector<Node*> children;
};

struct Token {
    string type;
    string lexeme;
};

class SyntaxAnalyzer {
private:



    bool match(string expectedType) {
        if (index < tokens.size() && tokens[index].type == expectedType) {
            index++;
            return true;
        }
        //cout<<index<<endl;
        return false;
    }

    Node* createNode(string label) {
        Node* newNode = new Node;
        newNode->label = label;
        return newNode;
    }

    void addChild(Node* parent, Node* child) {
        parent->children.push_back(child);
    }

Node* varNode() {
        string name ;
        if(syntax_semantic==0)name="VAR";
        else{name="VAR.value";}
         Node* Var = createNode(name);

         int i=0;
         bool flag=false;
        if (match("VAR")) {
            if(syntax_semantic==1){
            if(tokens[index-2].type != "INT" && tokens[index-2].type != "STRING_KEYWORD" && tokens[index-2].type != "FLOAT" && tokens[index-2-one_more_step_back_qoutation].type!="QOUTATION"  && tokens[index-2].type!= "COMMA"){

                   // cout<<"token"<<index-1<<" : "<<tokens[index-1].type<<endl;
                for(i;i<declared_variables.capacity();i++){
                   // cout<<declared_variables[i].variable<<endl;
                    if(declared_variables[i].variable == tokens[index-1].lexeme){
                    flag=true;
                    break;
                }
            }
        }
        else if(tokens[index-2-one_more_step_back_qoutation].type == "QOUTATION"  ){
            one_more_step_back_qoutation++;
            flag=true;
        }

            else{flag = true;}
            //cout<<"flag: "<<flag<<endl;
            if(!flag){
                cout<< "Variable : <"<< tokens[index-1].lexeme<<"> is not declared"<<endl;
            }
            }
            addChild(Var,createNode(tokens[index - 1].lexeme));
            addChild(Var,varNode());
            one_more_step_back_qoutation=0;

            return Var;
        }
         addChild(Var,createNode("~"));
         return Var;
    }

    Node* COND_OP() {
         Node* cond_op = createNode("COND_OP");
         addChild(cond_op,createNode(tokens[index - 1].lexeme));
         return cond_op;
    }


    Node* NUM() {
         Node* num = createNode("NUM");
         if(match("NUM")&& match("NUM")){

            addChild(num,DIGIT(2));
            addChild(num,NUM());
            return num;
         }
         else if(match("DIGIT") || !match("NUM")){
                addChild(num,DIGIT(1));
                return num;
         }
         return nullptr;
    }

        Node* DIGIT(int place) {
         Node* digit = createNode("Digit");
         addChild(digit,createNode(tokens[index - place].lexeme));
         return digit;
    }

    Node* conditionNode() {
        Node* condition = createNode("CONDITION");
        addChild(condition, varNode());
        if (match("COND_OP")) {
            addChild(condition, COND_OP());


            addChild(condition, NUM());

        }
        return condition;
    }


    Node* PLACE_HOLDER() {
         Node* place_holder = createNode("PLACE_HOLDER");
         addChild(place_holder,createNode(tokens[index - 1].lexeme));
         return place_holder;
    }

     Node* ESC_SEQ() {
         Node* esc_seq = createNode("ESC_SEQ");
         addChild(esc_seq,createNode(tokens[index - 1].lexeme));
         return esc_seq;
    }



     Node* STRING() {
         Node* s_tring = createNode("STRING");
        if (match("STRING")) {
            if(match("QOUTATION")){
                addChild(s_tring,createNode("\""));
            }
            addChild(s_tring,varNode());

            if(match("PLACE_HOLDER")){
                addChild(s_tring,PLACE_HOLDER());
                if(match("ESC_SEQ")){
                    addChild(s_tring,ESC_SEQ());
                    if(match("QOUTATION")){
                        addChild(s_tring,createNode("\""));
                    }
                }
            }
            else if(match("ESC_SEQ")){
                addChild(s_tring,ESC_SEQ());
                if(match("QOUTATION")){
                    addChild(s_tring,createNode("\""));
                }
            }




            return s_tring;
        }
         addChild(s_tring,createNode("~"));
         return s_tring;
    }




    Node* printfStatementNode() {
        Node* printfStatement = createNode("PRINTF_STATEMENT");
        if (match("PRINTF") && match("OPEN_PAREN") ) {
            addChild(printfStatement, createNode("Printf"));
            addChild(printfStatement, createNode("("));
            addChild(printfStatement,STRING());
            if (match("CLOSE_PAREN")) {
                addChild(printfStatement, createNode("CLOSE_PAREN"));
                if (match("SEMICOLON")) {
                    addChild(printfStatement,createNode(";"));
                }
            } else if (match("COMMA") ) {
                 addChild(printfStatement, createNode(","));
                addChild(printfStatement, varNode()); // Assuming VAR is the variable to be printed
                if(match("CLOSE_PAREN") && match("SEMICOLON")){
                addChild(printfStatement, createNode("CLOSE_PAREN"));
                addChild(printfStatement, createNode(";"));
                }
            }
             return printfStatement;
        }
       return nullptr;
    }

    Node* whileStatementNode() {
        Node* whileStatement = createNode("WHILE_STATEMENT");
        if (match("WHILE") && match("OPEN_PAREN")) {
            addChild(whileStatement,createNode("While"));
            addChild(whileStatement,createNode("("));
            addChild(whileStatement, conditionNode());

            if (match("CLOSE_PAREN") && match("OPEN_BRACE")) {
                addChild(whileStatement, createNode(")"));
                addChild(whileStatement,createNode("{"));
                addChild(whileStatement, bodyNode());
               // cout<<"closeparen!!"<<endl;
                if(match("CLOSE_BRACE")){
                        //cout<<"inwhilesemi"<<endl;
                    addChild(whileStatement,createNode("}"));
                }
            }
            return whileStatement;
        }
        return nullptr;
    }



     Node* EQ() {
         Node* eq = createNode("EQ");
         addChild(eq,createNode("="));
         return eq;
    }

    Node* OPERATOR() {
         Node* op = createNode("OPERATOR");
         addChild(op,createNode(tokens[index - 1].lexeme));
         return op;
    }

     Node* SEMICOLON() {
         Node* semicolon = createNode("SEMICOLON");
         addChild(semicolon,createNode(tokens[index - 1].lexeme));
         return semicolon;
    }

    Node* operationStatementNode() {
        Node* operationStatement = createNode("OPERATION_STATEMENT");
        addChild(operationStatement, varNode());

        if (match("EQ")) {
            addChild(operationStatement, EQ());
            if(match("VAR")){
             addChild(operationStatement, varNode()); // Assuming assignment operation with another variable
            }
            else if(match("NUM")){
                addChild(operationStatement, NUM());

                if(match("OPERATOR")){
                    addChild(operationStatement,OPERATOR());

                    if(match("NUM")){
                        addChild(operationStatement,NUM());

                    }

                    else if(match("VAR")){
                        addChild(operationStatement,varNode());

                        }
                    }
                }

            }

            else if(match("VAR")){
                addChild(operationStatement, varNode());

                if(match("OPERATOR")){
                    addChild(operationStatement,OPERATOR());

                    if(match("NUM")){
                        addChild(operationStatement,NUM());

                    }

                    else if(match("VAR")){
                        addChild(operationStatement,varNode());

                    }
                }
            }

            else if(match("OPERATOR")){
                    addChild(operationStatement,OPERATOR());

                    if(match("OPERATOR")){
                        addChild(operationStatement,OPERATOR());

                    }

            }
            if(match("SEMICOLON")){
                addChild(operationStatement,createNode(";"));


            return operationStatement;
        }
        return nullptr;

    }

    Node* commentStatementNode() {
        Node* commentStatement = createNode("COMMENT_STATEMENT");
        if (match("START_COMMENT") && match("COMMENT")&& match("END_COMMENT")) {
            addChild(commentStatement,createNode("/*"));
            addChild(commentStatement,createNode(tokens[index-2].lexeme));
            addChild(commentStatement,createNode("*/"));


            return commentStatement;
        }
        return nullptr;
    }

      Node* declarationStatementNode() {
        Node* declaration_statment= createNode("DECLARATION_STATMENT");
        if (match("INT")|| match("STRING_KEYWORD") || match("FLOAT")) {

            addChild(declaration_statment,createNode(tokens[index-1].lexeme));
            addChild(declaration_statment,varNode());
            if(match("EQ")){
                addChild(declaration_statment,EQ());
                 if (match("DIGIT")){
                    addChild(declaration_statment,DIGIT(1));
                    addChild(declaration_statment,createNode(";"));
                    declared_variables.push_back({tokens[index-5].type,tokens[index-4].lexeme});
                    //cout<<declared_variables[index_Data_Type_Variables].type<<declared_variables[index_Data_Type_Variables].variable<<endl;
                    index_Data_Type_Variables++;


                }
                else if (match("QOUTATION")){
                    addChild(declaration_statment,createNode("\""));
                    addChild(declaration_statment,varNode());
                    if(match("QOUTATION")){
                        addChild(declaration_statment,createNode("\""));
                        addChild(declaration_statment,createNode(";"));
                        declared_variables.push_back({tokens[index-7].type,tokens[index-6].lexeme});
                     //cout<<declared_variables[index_Data_Type_Variables].type<<declared_variables[index_Data_Type_Variables].variable<<endl;
                        index_Data_Type_Variables++;

                    }
                }
                else if (match("FLOATNUM")){
                    addChild(declaration_statment,createNode(tokens[index-1].lexeme));
                   addChild(declaration_statment,createNode(";"));
                   declared_variables.push_back({tokens[index-5].type,tokens[index-4].lexeme});
                    //cout<<declared_variables[index_Data_Type_Variables].type<<declared_variables[index_Data_Type_Variables].variable<<endl;
                   index_Data_Type_Variables++;


                }
                else{
                    addChild(declaration_statment,NUM());
                    addChild(declaration_statment,createNode(";"));
                    declared_variables.push_back({tokens[index-5].type,tokens[index-4].lexeme});
                    //cout<<declared_variables[index_Data_Type_Variables].type<<declared_variables[index_Data_Type_Variables].variable<<endl;
                    index_Data_Type_Variables++;

                }

            }



            return declaration_statment ;
        }
        return nullptr;
    }


    Node* bodyNode() {
        Node* body = createNode("BODY");
         //cout<<"body"<<endl;
        while (index < tokens.size() || index > tokens.size() ) {
            // cout<<"bodywhile"<<endl;
            Node* statement = nullptr;
            if ((statement = printfStatementNode()) || (statement = whileStatementNode()) || (statement = operationStatementNode()) || (statement = commentStatementNode()) || (statement = declarationStatementNode())) {
                addChild(body, statement);
                if(index== tokens.size()){
                    index++;
                }
                addChild(body,bodyNode());


            } else {
                addChild(body,createNode("~"));

                //cout<<index<<endl;

            }
            return body;
        }
        return nullptr;
    }



public:

    SyntaxAnalyzer(vector<Token>& tokenStream) : tokens(tokenStream), index(0), root(nullptr) {}

    Node* parse() {
        root = createNode("PROGRAM");
        addChild(root, bodyNode());
        return root;
    }

    vector<Token> tokens;
    size_t index;
    Node* root;
};

void printTreeToFile(Node* node,ofstream& outputFile, int depth = -1, bool lastChild = false) {
    if (node == nullptr) {
        return;
    }


    // Print the node label with appropriate indentation
    if(depth!=-1){
    cout << string(depth * 4, ' ');
    }

    if(node->label!="PROGRAM"){


    if (lastChild) {
        cout << "\\-- ";
    } else {
        cout << "|-- ";
    }

         cout << node->label << endl;
    }



    // Recursively print children
    for (size_t i = 0; i < node->children.size(); ++i) {
        printTreeToFile(node->children[i],outputFile, depth + 1, i == node->children.size() - 1); // Recursive call for children
    }

}

/*
void printTreeToFile(Node* node, ofstream& outputFile, int depth = -1, bool lastChild = false) {
    if (node == nullptr) {
        return;
    }

    // Print the node label with appropriate indentation
    if (depth != -1) {
        outputFile << string(depth * 4, ' ');
    }

    if (node->label != "PROGRAM") {
        if (lastChild) {
            outputFile << "\\-- ";
        } else {
            outputFile << "|-- ";
        }
        outputFile << node->label << endl;
    }

    // Recursively print children
    for (size_t i = 0; i < node->children.size(); ++i) {
        printTreeToFile(node->children[i], outputFile, depth + 1, i == node->children.size() - 1); // Recursive call for children
    }
}*/


int main() {
    // Same tokenStream initialization and parsing logic as before


   vector<Token> tokenStream = {
    {"START_COMMENT","/*"},
    {"COMMENT","hello my name is sakr"},
    {"END_COMMENT","*/"},
    {"WHILE","while"},
    {"OPEN_PAREN", "("},
    {"VAR","i"},
    {"COND_OP", "<"},
    {"NUM","2"},
    {"CLOSE_PAREN", ")"},
    {"OPEN_BRACE", "{"},
    {"PRINTF", "printf"},
    {"OPEN_PAREN", "("},
    {"STRING", "\"ab%d\\n\""},
    {"QOUTATION","\""},
    {"PLACE_HOLDER","%d"},
    {"ESC_SEQ","\n"},
    {"QOUTATION","\""},
    {"SEMICOLON", ";"},
     {"VAR", "i"},
    {"OPERATOR", "+"},
    {"OPERATOR", "+"},
       {"SEMICOLON", ";"},
        {"CLOSE_BRACE", "}"},






/*
    {"","i"},
    {"EQ","="},
    {"NUM","5"},
    {"NUM","8"},
    {"SEMICOLON",";"},
    {"VAR","i"},
    {"EQ","="},
    {"VAR","d"},
    {"OPERATOR","+"},
    {"DIGIT","2"},
    {"SEMICOLON",";"},
    {"STRING_KEYWORD","STRING"},
    {"VAR","p"},
    {"EQ","="},
    {"QOUTATION","\""},
    {"VAR","c"},
    {"VAR","c"},
    {"QOUTATION","\""},
    {"SEMICOLON",";"},
    {"WHILE", "while"},
    {"OPEN_PAREN", "("},
    {"VAR", "i"},
    {"COND_OP", "<"},
    {"DIGIT","6"},
    {"CLOSE_PAREN", ")"},
    {"OPEN_BRACE", "{"},
    {"PRINTF", "printf"},
    {"OPEN_PAREN", "("},
    {"STRING", "\"ab%d\\n\""},
    {"QOUTATION","\""},
    {"VAR","askjdk"},
    //{"VAR","b"},
    {"PLACE_HOLDER","%d"},
    {"ESC_SEQ","\n"},
    {"QOUTATION","\""},
    {"COMMA", ","},
    {"VAR", "i"},
    {"CLOSE_PAREN", ")"},
    {"SEMICOLON", ";"},
    {"VAR", "i"},
    {"OPERATOR", "+"},
    {"OPERATOR", "+"},
    {"SEMICOLON", ";"},
    {"VAR", "i"},
    {"EQ", "="},
    {"VAR", "8"},
    {"VAR", "i"},
    {"SEMICOLON", ";"},
    {"CLOSE_BRACE", "}"},
    */

};

cout<<"Syntax_tree : "<<endl;

for(int i=0;i<tokenStream.capacity();i++){

 if (tokenStream[i].type== "COND_OP"){
        cout<<"first"<<endl;
      cout<<"       "<<tokenStream[i].lexeme<<"\n";

    cout<<"   "<<tokenStream[i-1].lexeme;

    cout<<"           "<<tokenStream[i+1].lexeme<<"\n\n\n\n";
}
else if (tokenStream[i].type== "VAR"&&(tokenStream[i+1].lexeme== "+" && tokenStream[i+2].lexeme=="+")||(tokenStream[i].lexeme== "-" && tokenStream[i+1].lexeme=="-") ){
cout<<"second"<<endl;
    cout<<"       "<<"="<<"\n\n";

    cout<<"   "<<tokenStream[i].lexeme;

    cout<<"         "<<tokenStream[i+1].lexeme<<"\n";

    cout<<"        "<<tokenStream[i].lexeme; //took i or num or digit

    cout<<"            "<<"1"<<"\n\n\n\n";//took i or num or digit

}
else if (tokenStream[i].lexeme== "=" && tokenStream[i+2].type== "OPERATOR" ){
cout<<"third"<<endl;
    cout<<"       "<<tokenStream[i].lexeme<<"\n";

    cout<<"   "<<tokenStream[i-1].lexeme;

    cout<<"         "<<tokenStream[i+2].lexeme<<"\n";

    cout<<"        "<<tokenStream[i+1].lexeme; //took i or num or digit

    cout<<"            "<<tokenStream[i+2].lexeme<<"\n\n\n\n";//took i or num or digit



}
else if(tokenStream[i].lexeme== "=" && tokenStream[i-1].type=="VAR" &&(tokenStream[i-1].type=="VAR" || tokenStream[i-1].type=="NUM" || tokenStream[i-1].type=="DIGIT") ){
cout<<"fourth"<<endl;
    cout<<"       "<<tokenStream[i].lexeme<<"\n";

    cout<<"   "<<tokenStream[i-1].lexeme;
    if(tokenStream[i+1].type== "QOUTATION"){

    cout<<"         "<<tokenStream[i+2].lexeme<<"\n\n\n\n";
    }
    else{cout<<"         "<<tokenStream[i+1].lexeme<<"\n\n\n\n";}

}
}

ofstream output("Parse_tree_output.txt"); // Create&open the output file

    if (!output.is_open()) {
        cout << "Unable to open the output file." << endl;
        return 1;
    }
    //cout<<tokenStream.capacity();
    SyntaxAnalyzer parser(tokenStream);
    // cout<<"main"<<endl;
     //cout<<"\*"<<endl;

     for(syntax_semantic;syntax_semantic<2;syntax_semantic++){

    Node* parseTree = parser.parse();



   // cout<<parser.index<<endl;
    if (parseTree != nullptr && parser.index > parser.tokens.size()) {
        if(syntax_semantic==0){
        cout << "Parse is valid" <<"\n\n"<< endl;}
        else{cout<<"Semantic Tree:\n"<<endl;}
        printTreeToFile(parseTree,output);
        parser.index=0;
    } else {
        cout << "Parsing failed!" << endl;
    }
    }

    return 0;
}
