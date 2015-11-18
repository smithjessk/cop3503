#ifndef PA3_HEADER
#define PA3_HEADER

#include <string>

template <typename T>
class Node {
 private:
  T value;
  Node<T> *previous;

 public:
  Node(T value, Node<T> *previous);
  T get_value();
  Node<T> *get_previous();
};

template <typename T>
class Stack {
 private:
  Node<T> *head;
 
 public:
  Stack(T value);
  Node<T> *push(T value);
  Node<T> *pop();
};

struct Operator {
  std::string symbol;
};

struct BinaryOperator : Operator {};

struct SelfOperator : Operator {};

struct Operation {};

template <typename T>
struct SelfOperation : Operator {
  T self;
  SelfOperator op;
};

template <typename T, typename U>
struct BinaryOperation : Operator {
  T left;
  U right;
  BinaryOperator op;
};

#endif // PA3_HEADER