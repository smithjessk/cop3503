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

template <class T>
void LinkedList<T>::print() {
  Node<T> *current = getHead();
  std::cout << "Printing..." << std::endl;
  while (current != NULL) {
    std::cout << current->getValue() << std::endl;
    current = current->getNext();
  }
}

template <class T>
void LinkedList<T>::deleteNode(int index) {
  if (index < 0) {
    std::cout << "Cannot delete at indices less than zero" << std::endl;
    return; 
  }
  Node<T> *current = getHead();
  int numJumps = index;
  while (numJumps > 0) {
    if (current->getNext() == NULL) {
      std::cout << "Cannot delete at indices longer than the list" << 
        std::endl;
      return;
    }
    current = current->getNext();
    numJumps--;
  }
  std::cout << "Here is where we would delete the node with information " << 
    current->getValue() << std::endl;
  return;
}

struct ProgramInfo {
  int id;
  std::string name;

  ProgramInfo(int id, std::string name) {
    this->id = id;
    this->name = name;
  }
};

struct Chunk {
  int startPage, endPage;

  Chunk(int start, int end) {
    startPage = start;
    endPage = end;
  }
};

int main() {
  LinkedList<int> list;
  list.append(1);
  list.append(2);
  list.append(3);
  list.deleteNode(120);
  return 0;
}
