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
  int id;
  std::string name;

  ProgramInfo(int id, std::string name) {
    this->id = id;
    this->name = name;
  }
};

struct Chunk {
  int start_page, end_page;

  Chunk(int start, int end) {
    start_page = start;
    end_page = end;
  }
};

class MemoryAllocator {
 private:
  std::string algorithm;
  LinkedList<Chunk> free;
  LinkedList<Chunk> used;

 public:
  MemoryAllocator(std::string algorithm);

  /**
   * Try to find a place to put a program represented by pInfo. If there is not
   * enough free memory, nothing is done. Otherwise, split `free` in the 
   * appropriate way and add a node to `used`. The node put into `used` must be
   * at the correct location.
   * @param pInfo Information representing the program
   */
  void add_program(ProgramInfo pInfo);

  /**
   * Deallocate the memory used by the program with name `program_name`. 
   * Specifically, delete the corresponding node from `used` and update `free`.
   * If killing this program creates a new fragment, then a new node is added 
   * to `free`. Otherwise, the appropriate node in `free` is updated with 
   * either a lower `start_page` or a higher `end_page`. 
   * @param program_name If this is not a valid program name, then do nothing.
   */
  void kill_program(std::string program_name);

  /**
   * Print the number of fragments. A fragment is defined a chunk of 
   * contiguous memory; hence, the number of fragments is the number of nodes
   * in `free`. 
   */
  void print_fragmentation();

  /**
   * For each page, print either "Free" or the name of the program that 
   * occupies it.
   */
  void print_memory();
};

#endif // PA2_HEADER