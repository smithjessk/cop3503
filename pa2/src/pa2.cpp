#include <iostream>
#include <cstdio>

#include "pa2.h"

// Node methods

template <class T>
Node<T>::Node(T inputted_value):
  value(inputted_value),
  next(NULL) {}

template <class T>
T Node<T>::get_value() {
  return value;
}

template <class T>
Node<T> *Node<T>::get_next() {
  return next;
}

template <class T>
Node<T> *Node<T>::set_next(Node<T> *node) {
  next = node;
  return next;
}

// LinkedList methods

template <class T>
LinkedList<T>::LinkedList():
  head(NULL) {}

template <class T>
Node<T> *LinkedList<T>::get_head() {
  return head;
}

template <class T>
Node<T> *LinkedList<T>::append(T value) {
  if (head == NULL) {
    head = new Node<T>(value);
    return head;
  }
  Node<T> *temp = head;
  while (temp->get_next() != NULL) {
    temp = temp->get_next();
  }
  temp->set_next(new Node<T>(value));
  return temp->get_next();
}

template <class T>
void LinkedList<T>::print() {
  Node<T> *current = get_head();
  std::cout << "Printing..." << std::endl;
  while (current != NULL) {
    std::cout << current->get_value() << std::endl;
    current = current->get_next();
  }
}

template <class T>
void LinkedList<T>::delete_node(int index) {
  if (index < 0) {
    std::cout << "Cannot delete at indices less than zero" << std::endl;
    return; 
  }
  Node<T> *current = get_head();
  int numJumps = index;
  while (numJumps > 0) {
    if (current->get_next() == NULL) {
      std::cout << "Cannot delete at indices longer than the list" << 
        std::endl;
      return;
    }
    current = current->get_next();
    numJumps--;
  }
  std::cout << "Here is where we would delete the node with information " << 
    current->get_value() << std::endl;
  return;
}

// MemoryAllocator methods
MemoryAllocator::MemoryAllocator(std::string algorithm) {
  this->algorithm = algorithm;
  std::printf("Using %s fit algorithm\n", algorithm.c_str());
}

void print_instructions() {
  std::cout << 
    "1. Add program\n" <<
    "2. Kill program\n" <<
    "3. Fragmentation\n" <<
    "4. Print memory\n" <<
    "5. Exit\n" << std::endl;
}

int run_loop(std::string algorithm) {
  MemoryAllocator mem_alloc(algorithm);
  return 0;
}

int main(int argc, char** argv) {
  /*LinkedList<int> list;
  list.append(1);
  list.append(2);
  list.append(3);
  list.delete_node(120);*/
  std::string algorithm = argv[1];
  if (algorithm.compare("best") == 0) {
    return run_loop(algorithm);
  } else if (algorithm.compare("worst") == 0) {
    return run_loop(algorithm);
  } else {
    std::printf("Unknown algorithm \"%s\"\n", algorithm.c_str());
    std::cout << "First argument must be the algorithm to use " << 
      "(namely, \"best\" or \"worst\")" << std::endl;
    return 0;
  }
}
