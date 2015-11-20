#include <cstdio>
#include <iostream>

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
Stack<T>::Stack(T value) {
  this->head = new Node<T>(value, NULL);
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

bool is_numeric(char c) {
  return 48 <= (int)(c) && (int)(c) <= 57;
}

Token begin("");
Token end("");

Stack<Token> parse_line(std::string to_parse, Stack<Token> tokens) {
  if (to_parse.size() == 0) {
    return tokens;
  }
  if (is_numeric(to_parse[0])) {
    std::string text(1, to_parse[0]);
    to_parse = to_parse.substr(1);
    while (to_parse.size() > 0 && is_numeric(to_parse[0])) {
      text = text + to_parse[0];
      to_parse = to_parse.substr(1);
    }
    tokens.push(Constant(text));
    return parse_line(to_parse, tokens);
  }
  return tokens;
}

int main() {
  std::string to_parse = "123";
  Stack<Token> tokens(begin);
  tokens = parse_line(to_parse, tokens);
  tokens.push(end);
  std::cout << tokens.pop()->get_value().text << std::endl;
  std::cout << tokens.pop()->get_value().text << std::endl;
  std::cout << tokens.pop()->get_value().text << std::endl;
  return 0;
}