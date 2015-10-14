#ifndef PA2_HEADER
#define PA2_HEADER

template <class T>
class Node {
 private:
  T value;
  T *next;

 public:
  Node(T value);

  T setValue(T value);
  T *setNext(T *next);

  T getValue();
  T *getNext();
};

template <class T>
class LinkedList {
 private:
  Node<T> head;

 public:
  LinkedList(T value);
  Node<T> getHead();
  T addNode(T value);
};

#endif // PA2_HEADER