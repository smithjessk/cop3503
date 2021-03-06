#ifndef PA3_HEADER
#define PA3_HEADER

#include <string>
#include <set>

// Extended by all tokens
struct Token {
  std::string text;
  std::string type;
  Token(std::string text, std::string type) {
    this->text = text;
    this->type = type;
  }
};

// Set of tokens. 
struct Line {
  std::vector<Token> tokens;
};

// Set of lines.
struct CodeBlock {
  std::vector<Line> lines;

  void add_line(Line l) {
    lines.push_back(l);
  }
};

// E.g. BEGAN
struct Unexpected : Token {
  Unexpected(std::string in): Token(in, "unexpected") {}
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

struct LineWalker {
  std::vector<Token> tokens;
  std::vector<Token> missing;
  std::vector<Token> unexpected;
  int index;
  bool is_for_declaration;
  bool is_begin;
  bool is_end;
  LineWalker(std::vector<Token> tokens) {
    this->tokens = tokens;
    index = 0;
    is_for_declaration = false;
    is_begin = false;
    is_end = false;
  }
};

class ProgramWalker {
 private:
  int max_loop_depth;
  int num_for_declarations;
  int num_begins;
  int num_ends;
  std::set<std::string> keywords;
  std::set<std::string> identifiers;
  std::set<std::string> constants;
  std::set<std::string> operators;
  std::set<std::string> delimiters;
  std::set<std::string> unexpected;
  std::set<std::string> missing;

 public:
  ProgramWalker();
  // Return -1 if num_begins > num_ends, 0 if they're equal, and 1 if 
  // num_ends > num_begins
  int compare_n_begins_ends();

  // Incorporate a new parsed line and its tokens.
  void add_line(LineWalker lw);

  // Used in case we're missing begins or ends.
  void insert_missing(std::string s);

  void print_loop_depth();
  void print_keywords();
  void print_identifiers();
  void print_constants();
  void print_operators();
  void print_delimiters();

  // Separates syntax errors into missing and unexpected tokens.
  void print_syntax_errors();
};

#endif // PA3_HEADER