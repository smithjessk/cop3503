#include <iostream>
#include <string>

#include "pa2.h"

/*
template <class T>
LinkedList<T>::LinkedList(T inputtedValue) {
  value = inputtedValue;
}

template <class T>
T LinkedList<T>::getValue() {
  return value;
}*/

template <class T>
Node<T>::Node(T inputtedValue):
  value(inputtedValue) {}

template <class T>
T Node<T>::getValue() {
  return value;
}

template <class T>
LinkedList<T>::LinkedList(T value):
  head(Node<T> (value)) {}

template <class T>
Node<T> LinkedList<T>::getHead() {
  return head;
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
  LinkedList<ProgramInfo> list(pInfo);
  std::cout << list.getHead().getValue().id << std::endl;
  return 0;
}