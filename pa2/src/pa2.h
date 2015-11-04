#ifndef PA2_HEADER
#define PA2_HEADER

#include <string>

// Represents a node in a linked list.
template <class T>
class Node {
 private:
  T value;
  Node<T> *next;

 public:
  // Set `next = NULL`
  Node(T value);

  // Return `T`
  T set_value(T value);

  // Return `next`
  Node<T> *set_next(Node<T> *node);

  T get_value();
  Node<T> *get_next();
};

// Represents a singly linked list.
template <class T>
class LinkedList {
 private:
  Node<T> *head; // The first entry in this linked list

 public:
  LinkedList(); // Set head to `NULL`

  Node<T> *set_head(Node<T> *head);

  Node<T> *get_head();

  // Push a new node containing `value` to the end of the list. Return a 
  // pointer to the newly created node. 
  Node<T> *append(T value);
};

// Represents the passed in program information. 
struct ProgramInfo {
  std::string name;
  int size; // In KB

  ProgramInfo(std::string name, int size) {
    this->name = name;
    this->size = size;
  }
};

// Represents a set of pages in memory.
struct Chunk {
  int start_page, end_page; // Inclusive bounds

  Chunk(int start, int end) {
    start_page = start;
    end_page = end;
  }
};

// Represents a memory chunk that also keeps track of a program name. Hence it 
// is useful for the used-memory linked list.
struct UsedMemoryChunk : Chunk {
  std::string program_name;

  UsedMemoryChunk(int start, int end, std::string name): 
    Chunk(start_page, end_page) {
      start_page = start;
      end_page = end;
      program_name = name;
    }
};

// Represents the class the user interacts with. 
class MemoryAllocator {
 private:
  std::string algorithm; // Either "best" or "worst"
  LinkedList<Chunk> free_mem;
  LinkedList<UsedMemoryChunk> used_mem;

 public:
  MemoryAllocator(std::string algorithm, int num_pages);

  /**
   * Try to find a place to put a program represented by prog_info. If there
   * is not enough free memory, nothing is done. Otherwise, split `free_mem` in
   * the appropriate way and add a node to `used_mem`. The node put into 
   *`used_mem` must be at a logical location (start and end pages 
   * monotonically increasing). 
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
   * Defragment the free_mem list. That is, connect adjacent nodes `A` and 
   * `B` in `free_mem` if `A.end_page + 1` = `B.start_page`
   */
  void defragment();

  /**
   * Print the number of fragments. A fragment is defined a set of 
   * contiguous pages; hence, the number of fragments is the number of nodes
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