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

void jump(std::string &s, int size) {
  s = s.substr(size);
}

Token begin("\\BOF");
Token end("\\EOF");

Stack<Token> parse_line(std::string to_parse, Stack<Token> tokens) {
  if (to_parse.size() == 0) {
    return tokens;
  }
  char curr = to_parse[0];
  if (is_numeric(curr)) {
    std::string text(1, curr);
    jump(to_parse, 1);
    // Keep adding more numbers to this constant, if necessary
    while (to_parse.size() > 0) { 
      curr = to_parse[0];
      if (is_numeric(curr)) {
        text = text + curr;
        jump(to_parse, 1);
      } else {
        break;
      }
    }
    tokens.push(Constant(text));
    return parse_line(to_parse, tokens);
  } else if (is_space(curr)) {
    jump(to_parse, 1);
    return parse_line(to_parse, tokens);
  }
  return tokens;
}

int main() {
  std::string to_parse = "12 3 456";
  Stack<Token> tokens;
  tokens = parse_line(to_parse, tokens);
  Node<Token> *token = tokens.pop();
  while (token != NULL) {
    std::cout << token->get_value().text << std::endl;
    token = tokens.pop();
  }
  return 0;
}