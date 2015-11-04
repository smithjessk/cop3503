#include <cstdio>

#include "pa3.h"

template <class T>
Node<T>::Node(T value, Node<T> *previous) {
  this->value = value;
  this->previous = previous;
}

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

int main() {
  Stack<int> s(1);
  s.push(5);
  s.push(4);
  s.push(3);
  s.push(2);
  for (size_t i = 0; i < 3; i++) {
    std::printf("popped: %d\n", s.pop()->get_value());
  }
  s.push(100);
  s.push(101);
  for (size_t i = 0; i < 4; i++) {
    std::printf("popped: %d\n", s.pop()->get_value()); 
  }
  return 0;
}