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


};

#endif // PA2_HEADER