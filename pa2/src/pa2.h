#ifndef PA2_HEADER
#define PA2_HEADER

#include <string>

template <class T>
class Node {
 private:
  T value;
  Node<T> *next;

 public:
  Node(T value);

  T set_value(T value);
  Node<T> *set_next(Node<T> *node);

  T get_value();
  Node<T> *get_next();
};

template <class T>
class LinkedList {
 private:
  Node<T> *head;

 public:
  LinkedList();

  void print();

  Node<T> *get_head();

  Node<T> *append(T value);

  void delete_node(int index);

  void delete_node(bool (*shouldDelete)(T));
};

struct ProgramInfo {
  std::string name;
  int size; // In KB

  ProgramInfo(std::string name, int size) {
    this->name = name;
    this->size = size;
  }
};

struct Chunk {
  int start_page, end_page; // inclusive

  Chunk(int start, int end) {
    start_page = start;
    end_page = end;
  }
};

class MemoryAllocator {
 private:
  std::string algorithm;
  LinkedList<Chunk> free_mem;
  LinkedList<Chunk> used_mem;

 public:
  MemoryAllocator(std::string algorithm);

  /**
   * Try to find a place to put a program represented by prog_info. If there
   * is not enough free memory, nothing is done. Otherwise, split `free_mem` in
   * the appropriate way and add a node to `used_mem`. The node put into 
   *`used_mem` must be at the correct location.
   * @param prog_info Information representing the program
   */
  void add_program(ProgramInfo prog_info);

  /**
   * Deallocate the memory used by the program with name `program_name`. 
   * Specifically, delete the corresponding node from `used_mem` and update 
   * `free_mem`. If killing this program creates a new fragment, then a new 
   * node is added to `free_mem`. Otherwise, the appropriate node in `free_mem`
   * is updated with either a lower `start_page` or a higher `end_page`. 
   * @param program_name If this is not a valid program name, then do nothing.
   */
  void kill_program(std::string program_name);

  /**
   * Print the number of fragments. A fragment is defined a chunk of 
   * contiguous memory; hence, the number of fragments is the number of nodes
   * in `free_mem`. 
   */
  void print_fragmentation();

  /**
   * For each page, print either "Free" or the name of the program that 
   * occupies it.
   */
  void print_memory();
};

#endif // PA2_HEADER