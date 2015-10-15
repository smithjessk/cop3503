#ifndef PA2_HEADER
#define PA2_HEADER

template <class T>
class Node {
 private:
  T value;
  Node<T> *next;

 public:
  Node(T value);

  T setValue(T value);
  Node<T> *setNext(Node<T> *node);

  T getValue();
  Node<T> *getNext();

  // If numHops == 0, delete this node. Otherwise decrement numHops and pass it
  // along.
  void deleteNode(int numHops);

  // If false, pass to the next item in the list. Else, delete this node.
  void deleteNode(bool (*shouldDelete)(T));
};

template <class T>
class LinkedList {
 private:
  Node<T> *head;

 public:
  LinkedList();

  void print();

  Node<T> *getHead();

  Node<T> *append(T value);

  void deleteNode(int index);

  void deleteNode(bool (*shouldDelete)(T));
};

#endif // PA2_HEADER