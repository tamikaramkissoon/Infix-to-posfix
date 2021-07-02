#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;

#define MAXLEN 100

// stack of float data type
struct Stack {
  float element[MAXLEN];
  int top;

};

//function prototype for float stack
Stack * initStack();
bool isEmpty(Stack * s);
float peek(Stack * s);
void push(Stack * s, float n, ofstream &out);
float pop(Stack * s, ofstream &out);

// stack for char data type 
struct CharStack {
  char element[MAXLEN];
  int top;
};

//function prototype for char stack
CharStack * charStack();
bool isEmpty(CharStack * cs);
char peek(CharStack * cs);
void push(CharStack * cs, char n, ofstream &out);
char pop(CharStack * cs, ofstream &out);

// struct to store used to compare brackets/ determine a mismatch in brackets.
struct found {
  bool bracketFound;
  char character;

};

//prototypes of additional functions used within the program.
bool IsOperator(char ch);
bool IsOperand(char ch);
bool eqlOrhigher(char ch1, char ch2);
float precedence(char ch);
bool isDigit(char ch);
float PerformOperation(char operation, float operand1, float operand2, ofstream &out);
void toCString(string s, char c[]);
found compareBrackets(CharStack * cs, CharStack * bs, int size, ofstream &out);

int main() {
	
  //declaration and initialization of stacks	
  Stack * s = initStack();
  CharStack * cs = charStack();
  CharStack * bs = charStack();
  
  // declaration of file streams
  ifstream in ;
  ofstream out; in .open("input.txt");
  out.open("output.txt");

  // variables declarations and initializations
  string expression = "";
  char ch[100];
  int num = 0;
  for (int i = 0; i < 100; i = i + 1) {
    ch[i] = ' ';
  }
  float value = 0.0, val1 = 0.0, val2 = 0.0;

  // if statement to check whether the input file was successfully opened.
  if (! in .is_open()) {
    cout << "There was an error opening the data file!  Operation aborting!!..." << endl;
    out << "There was an error opening the data file!  Operation aborting!!..." << endl;
    exit(1);
  } else{
  		cout << "Input file sucessfully opened..." << endl;
  		out << "Input file sucessfully opened..." << endl;
  }
    

  cout << "\nStart of Assignment 3\n" << endl;
  out << "\nStart of Assignment 3\n" << endl;

  getline( in , expression); // reads  a string
  while (expression != "$") { //terminated by a line containing $ only
    s = initStack();
    cs = charStack();
    bs = charStack();
    cout << "Infix: ";
	out << "Infix: ";
	
    toCString(expression, ch); // converts string into c string
    int length = strlen(ch);
    for (int i = 0; i < length; i = i + 1) {
      cout << ch[i];
      out << ch[i];// output the infix as a cstring
    }
    cout << endl;
	out << endl;
	
    for (int i = 0; i < length; i = i + 1) {
      push(cs, ch[i], out);

    }
    
	found compare = compareBrackets(cs, bs, length, out); //checks for a mismatched bracket

    if (compare.bracketFound == true) {//Print a specific message if parentheses do not match.
      if (compare.character == ')') {
        cout << "Error: more right than left parentheses" << endl;
        out << "Error: more right than left parentheses" << endl;
      } else
      if (compare.character == '('){
      	cout << "Error: more left than right parentheses" << endl;
        out << "Error: more left than right parentheses" << endl;
	  }
        
    } else {
      if (compare.bracketFound == false) {
        cout << "Postfix: ";
        out << "Postfix: ";
        
		string postfix = "";
        char chh;
        push(s, '(', out); // converts infix to postfix
        expression = expression + ')';
        
		for (int i = 0; i < expression.length(); i = i + 1) {
          chh = expression[i];

          if (chh == ' ')
            continue;
          else if (chh == '(')
            push(s, chh, out);
          else if (IsOperand(chh))
            postfix = postfix + chh;
          else if (IsOperator(chh)) {
            while (!isEmpty(s) && eqlOrhigher(s -> element[s -> top], chh)) {
              postfix += s -> element[s -> top];
              pop(s, out);
            }
            push(s, chh, out);
          } else if (chh == ')') {
            while (!isEmpty(s) && s -> element[s -> top] != '(') {
              postfix += s -> element[s -> top];
              pop(s, out);
            }
            pop(s, out);
          }

        }
        cout << postfix << endl;
        out << postfix << endl;
       
	    cout << "Evaluation: ";
        out << "Evaluation: ";
       s = initStack();
        toCString(postfix, ch);
        int lengthh = strlen(ch);
        for (int i = 0; i < lengthh; i = i + 1) { // evaluates the posfix expression

          chh = ch[i];
          if (IsOperand(chh)) {
          	float digit = chh - '0';
            push(s, digit, out);
            
          } else {
            if (IsOperator(chh)) {
              val1 = pop(s, out);
              val2 = pop(s, out);

              if (chh == '/') {
                value = val2 / val1;
                push(s, value, out);
              } else {

                if (chh == '*') {
                  value = val2 * val1;
                  push(s, value, out);
                } else {

                  if (chh == '+') {
                    value = val2 + val1;
                    push(s, value, out);
                  } else {

                    if (chh == '-') {
                      value = val2 - val1;
                      push(s, value, out);
                      
                    } else {
                    	if (chh == '^') {
                      value = pow(val2, val1);
                      push(s, value, out);
					}
                  }
                }
              }
				}
            }
          }
        }
        float num = pop(s, out);
        cout << num;
        out << num;
      }
    }

    //
    cout << endl << endl;
    out << endl << endl;
    getline( in , expression);// reads in another string
  }
  in.close();
    return 0;
}

Stack * initStack() { // initialize the float stack
  Stack * s;

  s = (Stack * ) malloc(sizeof(Stack));

  s -> top = -1;

  return s;
}

bool isEmpty(Stack * s) { // checks if the float stack is empty

  return (s -> top == -1);
}

bool isFull(Stack * s) { // checks if the the float stack is full

  return (s -> top == MAXLEN - 1);
}

float peek(Stack * s, ofstream &out) { // checks the value at the top of the float stack

  if (isEmpty(s)) {

    cout << "Stack is empty." << endl;
	cout << "Stack is empty." << endl;
	
    return -999;

  }

  return s -> element[s -> top];
}

void push(Stack * s, float n, ofstream &out) { // addsa value to the top of the float stack

  if (isFull(s)) {

    cout << "Stack is full." << endl;
	out << "Stack is full." << endl;
    return;

  }

  s -> top = s -> top + 1;

  s -> element[s -> top] = n;

}

float pop(Stack * s, ofstream &out) { // removes a value from the top of the float stack

  float n;

  if (isEmpty(s)) {

    cout << "Stack is empty." << endl;
	out << "Stack is empty." << endl;
    return -999;

  }

  n = s -> element[s -> top];

  s -> top = s -> top - 1;

  return n;
}

bool IsOperator(char ch) { // checks to see if the char is a operator

  if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
    return true;
  else
    return false;
}

bool IsOperand(char ch) { // checks to see if the char is a operand
  if (ch >= '0' && ch <= '9') 
    return true;

  return false;
}

float precedence(char ch) { // determines order of precedence
  if (ch == '+' || ch == '-') //it defines the lowest precedence 
    return 1;
  else {
    if (ch == '*' || ch == '/')
      return 2;
    if (ch == '^') // exponent operator has the highest precedence 
      return 3;
  }
  return 0;
}

bool eqlOrhigher(char operand1, char operand2) {//determines if the precedence is equal or higher
  float precedence1 = precedence(operand1);
  float precedence2 = precedence(operand2);

  if (precedence1 == precedence2) {
    if (operand1 == '^')
      return false;
    else
      return true;
  }
  return (precedence1 > precedence2);
}

bool isDigit(char ch) {// determine is teh char is a digit

  if (ch >= '0' && ch <= '9')
    return true;
  else
    return false;
}

float PerformOperation(char operation, float operand1, float operand2, ofstream &out) { // performs arithmatic
  if (operation == '+')
    return operand1 + operand2;
  else if (operation == '-')
    return operand1 - operand2;
  else if (operation == '*')
    return operand1 * operand2;
  else if (operation == '/')
    return operand1 / operand2;
	else if(operation == '^')
	return pow(operand1, operand2);

  else {
  	cout << "Unexpected Error \n";
  	out << "Unexpected Error \n";
  }
  return -1;
}

void toCString(string s, char c[]) { // converts string into cstring

  int length = s.length();

  for (int i = 0; i < length; i = i + 1)
    c[i] = s[i];

  c[length] = '\0';
}

//fucntions for the char stack
CharStack * charStack() {

  CharStack * cs;
  cs = (CharStack * ) malloc(sizeof(CharStack));

  cs -> top = -1;

  return cs;
}

bool isEmpty(CharStack * cs) {

  return (cs -> top == -1);
}

bool isFull(CharStack * cs) {

  return (cs -> top == MAXLEN - 1);
}

char peek(CharStack * cs, ofstream &out) {

  if (isEmpty(cs)) {

    cout << "Stack is empty." << endl;
	out << "Stack is empty." << endl;
    return '0';

  }

  return cs -> element[cs -> top];
}

void push(CharStack * cs, char n, ofstream &out) {

  if (isFull(cs)) {

    cout << "Stack is full." << endl;
	out << "Stack is full." << endl;
    return;

  }

  cs -> top = cs -> top + 1;

  cs -> element[cs -> top] = n;
}

char pop(CharStack * cs, ofstream &out) {

  char n;
  if (isEmpty(cs)) {

    cout << "Stack is empty." << endl;
	out << "Stack is empty." << endl;
    return '0';

  }
  n = cs -> element[cs -> top];

  cs -> top = cs -> top - 1;
  return n;
}

found compareBrackets(CharStack * cs, CharStack * bs, int size, ofstream &out) { // determines if there ia a mismatch of brackets and return true 
																				//or false and the bracket that is mismatched, if any.
  int num1 = 0, num2 = 0;	
  found brackets;
  char ch = ' ';
 
  
  for (int i = 1; i <= size; i = i + 1){
  	ch = pop(cs, out);
 
  	if (ch == '(' ){
  		push(bs, ch, out);
  		num1 = num1 + 1;
	  }
	else{
		if (ch == ')'){
			push(bs,ch, out);
			num2 = num2 + 1;
		}
	}
  }

int num3 = num1 + num2;

  if (num3 % 2 == 0) {
    brackets.bracketFound = false;

    brackets.character = ' ';
  } 
  
  	if(num1 > num2){
	   brackets.bracketFound = true;
	   brackets.character = '(';
  } else {
  		if(num1 < num2){
  			brackets.bracketFound = true;
  			brackets.character = ')';
		  }
  }

  return brackets;
}
  
