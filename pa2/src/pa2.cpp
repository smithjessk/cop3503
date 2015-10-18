#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

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
T Node<T>::set_value(T value) {
  this->value = value;
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

template <class T>
void LinkedList<T>::apply(void (*func)(Node<T>*)) {
  Node<T> *current = get_head();
  while (current != NULL) {
    (*func)(current);
    current = current->get_next();
  }
}

// MemoryAllocator methods
MemoryAllocator::MemoryAllocator(std::string algorithm) {
  this->algorithm = algorithm;
  std::printf("Using %s fit algorithm\n", algorithm.c_str());
  this->free_mem = LinkedList<Chunk>();
  free_mem.append(Chunk(0, 31));
  this->used_mem = LinkedList<Chunk>();

}

typedef std::pair<Node<Chunk>*, int> MyPairType;
struct CompareSecond
{
    bool operator()(const MyPairType& left, const MyPairType& right) const
    {
        return left.second < right.second;
    }
};


void MemoryAllocator::add_program(ProgramInfo prog_info) {
  if (prog_info.size <= 0) {
    std::cout << "Not a valid size" << std::endl;
    return;
  }

  std::map<Node<Chunk>*, int> free_slots;
  Node<Chunk> *current = free_mem.get_head();
  int num_pages = ceil(prog_info.size / 4.0);

  // std::printf("Num pages: %d\n", num_pages);

  while (current != NULL) {
    int free_size = current->get_value().end_page - 
      current->get_value().start_page + 1;
    if (free_size >= (num_pages)) {
      free_slots[current] = free_size;
    }
    current = current->get_next();
  }
  if (free_slots.size() == 0){
    std::cout << "Not enough memory" << std::endl;
    return;

  }
  if (this->algorithm == "best") {
    std::pair<Node<Chunk>*, int> min = 
      *std::min_element(free_slots.begin(), free_slots.end(), CompareSecond());

    Chunk smallestChunk = min.first->get_value();

    min.first->set_value(Chunk(smallestChunk.start_page + num_pages, 
      smallestChunk.end_page));

    /*std::printf("Smallest diff: %d\n", smallestChunk.end_page - 
      smallestChunk.start_page);*/

    Chunk allocatedMemory(smallestChunk.start_page, smallestChunk.start_page + 
      num_pages - 1);
    Node<Chunk> *new_node = new Node<Chunk>(allocatedMemory);

    // Put an entry in the used
    current = used_mem.get_head();
    Node<Chunk>* last = used_mem.get_head();
    while (current != NULL) {
      // Keep going until you pass the starting pointer of allocatedMemory
      if (current->get_value().start_page > allocatedMemory.start_page) {
        last->set_next(new_node);
        new_node->set_next(current);
        std::printf("Program %s added successfully: %d page(s) used\n\n", 
          prog_info.name.c_str(), num_pages);
        return;
      } else {
        // std::cout << "Jumping" << std::endl;
        last = current;
        current = current->get_next();
      }
    }
    used_mem.append(allocatedMemory);
    std::printf("Program %s added successfully: %d page(s) used\n\n", 
      prog_info.name.c_str(), num_pages);
  } else {
    // do something similar
  }
}

void MemoryAllocator::print_fragmentation() {
  int num_fragments = 0;
  Node<Chunk> *current = free_mem.get_head();
  while (current != NULL) {
    num_fragments++;
    current = current->get_next();
  }
  std::printf("There are %d fragment(s)\n\n", num_fragments);
}

void func(Node<Chunk> *current) {
  Chunk info = current->get_value();
  printf("Start: %d, End: %d\n", info.start_page, info.end_page);
}

void MemoryAllocator::print_memory() {
  std::printf("Free memory map:\n");
  free_mem.apply(func);
  std::printf("\nUsed memory map:\n");
  used_mem.apply(func);
  std::map<int, std::string> used_pages; // Page to program name
  Node<Chunk> *current = used_mem.get_head();
  while (current != NULL) {
    Chunk info = current->get_value();
    for (int i = info.start_page; i <= info.end_page; i++) {
      used_pages[i] = "Occupado";
    }
    current = current->get_next();
  }
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 8; col++) {
      int curr_page = row * 8 + col;
      if (used_pages.find(curr_page) != used_pages.end()) {
        std::cout << used_pages[curr_page] << " ";
      } else {
        std::cout << "Free ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void print_instructions() {
  std::cout << 
    "1. Add program\n" <<
    "2. Kill program\n" <<
    "3. Fragmentation\n" <<
    "4. Print memory\n" <<
    "5. Exit\n" << std::endl;
}

int get_choice() {
  int choice;
  std::cout << "Choice - ";
  std::cin >> choice;
  std::cout << std::endl;
  return choice;
}

std::string get_program_name() {
  std::string name;
  std::cout << "Program name - ";
  std::cin >> name;
  return name;
}

ProgramInfo get_program_info() {
  std::string name = get_program_name();
  int size;
  std::cout << "Program size (KB) - ";
  std::cin >> size;
  return ProgramInfo(name, size);
}

int run_loop(std::string algorithm) {
  MemoryAllocator mem_alloc(algorithm);

  print_instructions();
  int action_choice = -1;
  while (action_choice != 5) {
    action_choice = get_choice();
    switch (action_choice) {
      case 1:
        mem_alloc.add_program(get_program_info());
        break;
      case 2:
        // mem_alloc.kill_program(get_program_name());
        break;
      case 3:
        mem_alloc.print_fragmentation();
        break;
      case 4:
        mem_alloc.print_memory();
        break;
      case 5: // Will exit next time
        break;
      default:
        std::cout << "Unknown option " << std::endl;
        return 0;
    }
  }
  return 0;
}

int main(int argc, char** argv) {
  std::string algorithm = argv[1];
  if (algorithm.compare("best") == 0 || algorithm.compare("worst") == 0) {
    return run_loop(algorithm);
  } else {
    std::printf("Unknown algorithm \"%s\"\n", algorithm.c_str());
    std::cout << "First argument must be the algorithm to use " << 
      "(namely, \"best\" or \"worst\")" << std::endl;
    return 0;
  }
}
