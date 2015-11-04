#ifndef PA3_HEADER
#define PA3_HEADER

template <class T>
class Node {
 private:
  T value;
  Node<T> *previous;

 public:
  Node(T value, Node<T> *previous);
  T get_value();
  Node<T> *get_previous();
};

template <class T>
class Stack {
 private:
  Node<T> *head;
 
 public:
  Stack(T value);
  Node<T> *push(T value);
  Node<T> *pop();
};

#endif // PA3_HEADER