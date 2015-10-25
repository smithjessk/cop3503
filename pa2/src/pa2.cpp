#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

#include "pa2.h"

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

template <class T>
LinkedList<T>::LinkedList():
  head(NULL) {}

template <class T>
Node<T> *LinkedList<T>::get_head() {
  return head;
}

template <class T>
Node<T> *LinkedList<T>::set_head(Node<T> *head) {
  this->head = head;
  return this->head;
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
    std::printf("Cannot delete at indices less than zero\n");
    return; 
  }
  Node<T> *current = get_head();
  int numJumps = index;
  while (numJumps > 0) {
    if (current->get_next() == NULL) {
      std::printf("Cannot delete at indices longer than the list\n");
      return;
    }
    current = current->get_next();
    numJumps--;
  }
  delete *current;
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

void print_bounds(Node<Chunk> *current) {
  Chunk info = current->get_value();
  printf("Start: %d, End: %d\n", info.start_page, info.end_page);
  printf("This addr: %p\n", (void *)current);
  printf("Next addr: %p\n", (void *)current->get_next());
}

void print_bounds(Node<UsedMemoryChunk> *current) {
  UsedMemoryChunk info = current->get_value();
  printf("Start: %d, End: %d\n", info.start_page, info.end_page);
  printf("This addr: %p\n", (void *)current);
  printf("Next addr: %p\n", (void *)current->get_next());
}

// MemoryAllocator methods
MemoryAllocator::MemoryAllocator(std::string algorithm) {
  this->algorithm = algorithm;
  printf("Using %s fit algorithm\n", algorithm.c_str());
  this->free_mem = LinkedList<Chunk>();
  free_mem.append(Chunk(0, 31));
  this->used_mem = LinkedList<UsedMemoryChunk>();

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
    std::printf("Not a valid size\n");
    return;
  }

  std::map<Node<Chunk>*, int> free_slots;
  Node<Chunk> *current = free_mem.get_head();
  int num_pages = ceil(prog_info.size / 4.0);

  while (current != NULL) {
    int free_size = current->get_value().end_page - 
      current->get_value().start_page + 1;
    if (free_size >= (num_pages)) {
      free_slots[current] = free_size;
    }
    current = current->get_next();
  }

  if (free_slots.size() == 0){
    std::printf("Not enough memory\n");
    return;
  }

  Node<UsedMemoryChunk> *new_node = NULL;
  Node<Chunk> *node_to_use = NULL;
  if (this->algorithm == "best") {
    std::pair<Node<Chunk>*, int> min = 
      *std::min_element(free_slots.begin(), free_slots.end(), CompareSecond());
    node_to_use = min.first;
  } else { // Worst fit
    std::pair<Node<Chunk>*, int> max = 
      *std::max_element(free_slots.begin(), free_slots.end(), CompareSecond());
    node_to_use = max.first;
  }
  
  // It's possible that
  // chunk_to_use.start_page + num_pages > chunk_to_use.end_page
  // In this case, we do not want to include it in the free memory list 
  // because it is a chunk that makes no sense. 
  // Hence, we will not include this chunk. 
  Chunk chunk_to_use = node_to_use->get_value();
  if (chunk_to_use.start_page + num_pages > chunk_to_use.end_page) {
    if (free_mem.get_head() == node_to_use) {
      free_mem.set_head(node_to_use->get_next());
    } else {
      bool previous_node_found = false;
      current = free_mem.get_head()->get_next();
      while (current != NULL && !previous_node_found) {
        if (current->get_next() == node_to_use) {
          current->set_next(node_to_use->get_next());
          previous_node_found = true;
        } else {
          current = current->get_next();
        }
      }
    }
  } else {
    node_to_use->set_value(Chunk(chunk_to_use.start_page + num_pages, 
      chunk_to_use.end_page));  
  }

  int chunk_start = chunk_to_use.start_page,
    chunk_end = chunk_to_use.start_page + num_pages - 1;
  new_node = new Node<UsedMemoryChunk>(UsedMemoryChunk(chunk_start, chunk_end, prog_info.name));

  // Put an entry in the used
  
  Node<UsedMemoryChunk>* used_current = used_mem.get_head();
  Node<UsedMemoryChunk>* used_last = used_mem.get_head();
  if (used_current != NULL && 
    used_current->get_value().start_page > new_node->get_value().start_page) { // or .end_page?
    new_node->set_next(used_mem.get_head());
    used_mem.set_head(new_node);
    printf("Program %s added successfully: %d page(s) used\n\n", 
        prog_info.name.c_str(), num_pages);
    return;
  }

  while (used_current != NULL) {
    // Keep going until you pass the starting page of the allocated memory
    if (used_current->get_value().start_page > 
      new_node->get_value().start_page) {
      used_last->set_next(new_node);  
      new_node->set_next(used_current);
      printf("Program %s added successfully: %d page(s) used\n\n", 
        prog_info.name.c_str(), num_pages);
      return;
    } else {
      used_last = used_current;
      used_current = used_current->get_next();
    }
  }
  // In case we need to append at the end
  used_mem.append(new_node->get_value());
  printf("Program %s added successfully: %d page(s) used\n\n", 
    prog_info.name.c_str(), num_pages);
}

void MemoryAllocator::defragment() {
  Node<Chunk> *current = free_mem.get_head(),
    *last = NULL;
  while (current != NULL) {
    // Collapse to the left by seeing if prev.end_page + 1 = curr.start_page,
    if (last == NULL) {
      last = current;
      current = current->get_next();
    } else {
      int last_end = last->get_value().end_page,
      current_start = current->get_value().start_page;  
      if (last_end + 1 == current_start) {
        Chunk expanded(last->get_value().start_page, 
          current->get_value().end_page);
        last->set_value(expanded);
        last->set_next(current->get_next());
        return;
      } else {
        last = current;
        current = current->get_next();
      }
    }
  }
}

void MemoryAllocator::kill_program(std::string program_name) {
  int freed_start_page, freed_end_page;
  bool program_found = false;
  Node<UsedMemoryChunk> *used_current = used_mem.get_head();
  Node<UsedMemoryChunk> *used_last = NULL;

  while (used_current != NULL && !program_found) {
    UsedMemoryChunk current_chunk = used_current->get_value();
    if (program_name.compare(current_chunk.program_name) == 0) {
      freed_start_page = current_chunk.start_page;
      freed_end_page = current_chunk.end_page;
      if (used_last != NULL) {
        used_last->set_next(used_current->get_next());
      } else {
        used_mem.set_head(used_current->get_next());
      }
      program_found = true;
    } else {
      used_last = used_current;
      used_current = used_current->get_next();
    }
  }

  if (!program_found) {
    printf("Could not find program with name %s\n", program_name.c_str());
    return;
  }

  Chunk freed(used_current->get_value().start_page,
    used_current->get_value().end_page);
  Node<Chunk> *new_node = new Node<Chunk>(freed);

  if (free_mem.get_head() == NULL) {
    free_mem.set_head(new_node);
    std::printf("%s deleted. %d page(s) freed\n\n", program_name.c_str(), 
      freed_end_page - freed_start_page + 1);
    return;
  }

  // Keep going over free_mem until you pass the starting page of the freed
  // memory (via the start_page value on the chunk). Then, create a new node
  // and have it point to the first node with start_page greater than the freed
  // program's end_page. 
  Node<Chunk> *free_current = free_mem.get_head();
  Node<Chunk> *free_last = NULL;
  while (free_current != NULL) {
    int current_start_page = free_current->get_value().start_page;
    if (current_start_page > freed.start_page) {
      new_node->set_next(free_current);
      if (free_last == NULL) {
        free_mem.set_head(new_node);
      } else { 
        free_last->set_next(new_node);
      }
      defragment();
      printf("%s deleted. %d page(s) freed\n\n", program_name.c_str(), 
        freed_end_page - freed_start_page + 1);
      return;
    } else {
      free_last = free_current;
      free_current = free_current->get_next();
    }
  }

  printf("Something went wrong...?\n");
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

void MemoryAllocator::print_memory() {
  std::map<int, std::string> used_pages; // Page index to program name
  Node<UsedMemoryChunk> *current = used_mem.get_head();
  while (current != NULL) {
    UsedMemoryChunk info = current->get_value();
    for (int i = info.start_page; i <= info.end_page; i++) {
      used_pages[i] = info.program_name;
    }
    current = current->get_next();
  }
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 8; col++) {
      int curr_page = row * 8 + col;
      if (used_pages.find(curr_page) != used_pages.end()) {
        std::printf("%s ", used_pages[curr_page].c_str());
      } else {
        std::printf("Free ");
      }
    }
    std::printf("\n");
  }
  std::printf("\n");
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
  std::printf("Choice - ");
  std::cin >> choice;
  std::printf("\n");
  return choice;
}

std::string get_program_name() {
  std::string name;
  std::printf("Program name - ");
  std::cin >> name;
  return name;
}

ProgramInfo get_program_info() {
  std::string name = get_program_name();
  int size;
  std::printf("Program size (KB) - ");
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
        mem_alloc.kill_program(get_program_name());
        break;
      case 3:
        mem_alloc.print_fragmentation();
        break;
      case 4:
        mem_alloc.print_memory();
        break;
      case 5:
        break;
      default:
        std::printf("Unknown option\n");
        return 0;
    }
  }
  return 0;
}

int main(int argc, char** argv) {
  if (argv[1] != NULL) {
    std::string algorithm = argv[1];
    if (algorithm.compare("best") == 0 || algorithm.compare("worst") == 0) {
      return run_loop(algorithm);
    }
  }
  printf("First argument must be the algorithm to use (namely, ");
  printf("\"best\" or \"worst\")\n");
  return 0;
}
