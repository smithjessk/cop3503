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
  T deleteNode(int numHops);

  // If false, pass to the next item in the list. Else, delete this node.
  T deleteNode(bool (*shouldDelete)(T));
};

template <class T>
class LinkedList {
 private:
  Node<T> *head;

 public:
  LinkedList();

  Node<T> *getHead();

  Node<T> *append(T value);

  T deleteNode(int index);

  T deleteNode(bool (*shouldDelete)(T));
};

#endif // PA2_HEADER