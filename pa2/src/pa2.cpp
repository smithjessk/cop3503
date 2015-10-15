#include <iostream>
#include <string>

#include "pa2.h"

// Node methods

template <class T>
Node<T>::Node(T inputtedValue):
  value(inputtedValue),
  next(NULL) {}

template <class T>
T Node<T>::getValue() {
  return value;
}

template <class T>
Node<T> *Node<T>::getNext() {
  return next;
}

template <class T>
Node<T> *Node<T>::setNext(Node<T> *node) {
  next = node;
  return next;
}

// LinkedList methods

template <class T>
LinkedList<T>::LinkedList():
  head(NULL) {}

template <class T>
Node<T> *LinkedList<T>::getHead() {
  return head;
}

template <class T>
Node<T> *LinkedList<T>::append(T value) {
  if (head == NULL) {
    head = new Node<T>(value);
    return head;
  }
  Node<T> *temp = head;
  while (temp->getNext() != NULL) {
    temp = temp->getNext();
  }
  temp->setNext(new Node<T>(value));
  return temp->getNext();
}

struct ProgramInfo {
  int id;
  std::string name;

  ProgramInfo(int id, std::string name) {
    this->id = id;
    this->name = name;
  }
};

int main() {
  ProgramInfo pInfo(1, "a");
  LinkedList<ProgramInfo> list;
  list.append(pInfo);
  list.append(pInfo);
  std::cout << list.getHead()->getNext()->getValue().id << std::endl;
  return 0;
}