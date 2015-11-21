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
  Stack();
  Node<T> *push(T value);
  Node<T> *pop();
};

// Extended by all tokens
struct Token {
  std::string text;
  std::string type;
  Token(std::string text, std::string type) {
    this->text = text;
    this->type = type;
  }
};

struct Line {
  std::vector<Token> tokens;
};

struct CodeBlock {
  std::vector<Line> lines;

  void add_line(Line l) {
    lines.push_back(l);
  }
};

// E.g. a b cdef
struct Identifier : Token {
  Identifier(std::string in): Token(in, "identifier") {}
};

// E.g. 1 2 345
struct Constant : Token {
  Constant(std::string in): Token(in, "constant") {}
};

// I.e., FOR BEGIN END
struct Keyword : Token {
  Keyword(std::string in): Token(in, "keyword") {}
};

// I.e., ( ) ; ,
struct Delimiter : Token {
  Delimiter(std::string in): Token(in, "delimiter") {}
};

// Both binary and self (hence the two-argument constructor).
struct Operator : Token {
  Operator(std::string in, std::string type): Token(in, type) {}
};

// I.e., + - * / = 
struct BinaryOperator : Operator {
  BinaryOperator(std::string in): Operator(in, "binary_operator") {}
};

// I.e., ++
struct SelfOperator : Operator {
  SelfOperator(std::string in): Operator(in, "self_operator") {}
};

struct Expression {};

struct ForDeclaration : Expression {
  Identifier counter_var;
  Constant upper_limit;
  Operator oper;
  ForDeclaration(Identifier i, Constant c, Operator o):
    counter_var(i),
    upper_limit(c),
    oper(o) {}
};

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

struct LineWalker {
  std::vector<Token> tokens;
  std::vector<Token> missing;
  std::vector<Token> unexpected;
  int index;
  LineWalker(std::vector<Token> tokens) {
    this->tokens = tokens;
    index = 0;
  }
};

#endif // PA3_HEADER