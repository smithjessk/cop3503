#include <cstdio>
#include <iostream>
#include <cstring>

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

bool is_space(char c) {
  return c == ' ';
}

bool is_numeric(char c) {
  return 48 <= (int)(c) && (int)(c) <= 57;
}

// Returns true if the token is used inside any of the operator tokens. Note 
// that this includes = and the + inside ++. 
bool is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

void jump(std::string &s, int size) {
  s = s.substr(size);
}

Token begin("\\BOF");
Token end("\\EOF");

// Called when to_parse[0] is numeric
void handle_numbers(std::string &to_parse, Stack<Token> &tokens) {
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

Stack<Token> parse_line(std::string to_parse, Stack<Token> tokens) {
  if (to_parse.size() == 0) {
    return tokens;
  }
  char curr = to_parse[0];
  if (is_numeric(curr)) {
    handle_numbers(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_operator(curr)) {
    handle_operator(to_parse, tokens);
    return parse_line(to_parse, tokens);
  } else if (is_space(curr)) {
    jump(to_parse, 1);
    return parse_line(to_parse, tokens);
  } else {
    std::cerr << "Encountered unknown token: " << curr << std::endl;
  }
  return tokens;
}

int main() {
  std::string to_parse = "1 + 2 - 5 ++";
  Stack<Token> tokens;
  tokens = parse_line(to_parse, tokens);
  Node<Token> *token = tokens.pop();
  while (token != NULL) {
    std::cout << token->get_value().text << std::endl;
    token = tokens.pop();
  }
  return 0;
}