#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>

#include "pa3.h"

template <class T>
Node<T>::Node(T value, Node<T> *previous):
  value(value),
  previous(previous) {}

template<class T>
T Node<T>::get_value() {
  return this->value;
}

template<class T>
Node<T> *Node<T>::get_previous() {
  return this->previous;
}

template <class T>
Stack<T>::Stack() {
  this->head = NULL;
}

template <class T>
Node<T> *Stack<T>::push(T value) {
  Node<T> *prev = head;
  this->head = new Node<T>(value, prev);
  return this->head;
}

template <class T>
Node<T> *Stack<T>::pop() {
  if (this->head == NULL) {
    return NULL;
  }
  Node<T> *old_head = this->head;
  this->head = old_head->get_previous();
  return old_head;
}

bool in_ascii_ranges(char c, int lower, int upper) {
  return lower <= (int)(c) && (int)(c) <= upper;
}

bool equals_str(std::string a, std::string b) {
  return 0 == std::strcmp(a.c_str(), b.c_str());
}

bool is_space(char c) {
  return c == ' ' || c == '\t';
}

bool is_numeric(char c) {
  return in_ascii_ranges(c, 48, 57);
}

// Returns true if the token is used inside any of the operator tokens. Note 
// that this includes = and the + inside ++. 
bool is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

bool is_letter(char c) {
  return in_ascii_ranges(c, 65, 90) || in_ascii_ranges(c, 97, 122);
}

bool is_keyword(std::string s) {
  return equals_str(s, "FOR") || equals_str(s, "BEGIN") || 
    equals_str(s, "END");
}

bool is_delimiter(char c) {
  return c == '(' || c == ')' || c == ';' || c == ',';
}

void jump(std::string &s, int size) {
  s = s.substr(size);
}

Token begin("\\BOF");
Token end("\\EOF");

// Called when to_parse[0] is numeric
void handle_number(std::string &to_parse, Stack<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  while (to_parse.size() > 0) { // Append more numbers as necessary
    curr = to_parse[0];
    if (is_numeric(curr)) {
      text = text + curr;
      jump(to_parse, 1);
    } else { // No more numbers to gobble up
      break;
    }
  }
  tokens.push(Constant(text));
}

// Possible issue: This doesn't really do anything if there are two operators 
// not separated by whitespace that should be. For instance, this doesn't 
// really care about +-
void handle_operator(std::string &to_parse, Stack<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  if (curr == '+' && to_parse[0] == '+') {
    text = text + '+';
    jump(to_parse, 1);
    tokens.push(SelfOperator(text));
  } else {
    tokens.push(BinaryOperator(text));
  }
}

// Called when to_parse[0] is a letter
void handle_letter(std::string &to_parse, Stack<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  while (to_parse.size() > 0) { // Append more letters as necessary
    curr = to_parse[0];
    if (is_letter(curr)) {
      text += curr;
      jump(to_parse, 1);
    } else {
      break;
    }
  }
  if (is_keyword(text)) {
    tokens.push(Keyword(text));
  } else {
    tokens.push(Identifier(text));
  }
}

void handle_delimiter(std::string &to_parse, Stack<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  tokens.push(Delimiter(text));
}

void parse_line(std::string &to_parse, Stack<Token> &tokens) {
  if (to_parse.size() == 0) {
    return;
  }
  char curr = to_parse[0];
  if (is_numeric(curr)) { // Numbers
    handle_number(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_operator(curr)) { // Operators
    handle_operator(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_letter(curr)) { // Letters
    handle_letter(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_delimiter(curr)) {
    handle_delimiter(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_space(curr)) { // Spaces
    jump(to_parse, 1);
    return parse_line(to_parse, tokens);
  } else {
    std::cerr << "Encountered unknown token: " << curr << std::endl;
    jump(to_parse, 1);
    return parse_line(to_parse, tokens);
  }
}

std::string read_line(std::ifstream &ifs) {
  char line[256];
  ifs.getline(line, 256);
  return std::string(line);
}

int main(int argc, char **argv) {
  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  CodeBlock program;
  while (!ifs.eof()) { // While there are more lines
    Line line;
    std::string to_parse = read_line(ifs);
    parse_line(to_parse, line.tokens);
    program.add_line(line);
  }
  return 0;
}