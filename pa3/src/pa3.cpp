#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>

#include "pa3.h"

ProgramWalker::ProgramWalker() {
  this->max_loop_depth = 0;
  this->num_for_declarations = 0;
  this->num_begins = 0;
  this->num_ends = 0;
}

void ProgramWalker::add_line(LineWalker lw) {
  if (lw.is_for_declaration) {
    this->num_for_declarations++;
  } else if (lw.is_begin) {
    this->num_begins++;
  } else if (lw.is_end) {
    this->num_ends++;
  }
  this->max_loop_depth = std::max(this->max_loop_depth, 
    this->num_for_declarations - this->num_ends); 
}

void ProgramWalker::print_loop_depth() {
  if (this->num_for_declarations == 0) {
    std::printf("There were no loops in this program\n");
  } else {
    std::printf("Depth of nested loops was %d\n", max_loop_depth);
  }
}

bool in_ascii_range(char c, int lower, int upper) {
  return lower <= (int)(c) && (int)(c) <= upper;
}

bool equals_str(std::string a, std::string b) {
  return 0 == std::strcmp(a.c_str(), b.c_str());
}

bool is_space(char c) {
  return c == ' ' || c == '\t';
}

bool is_numeric(char c) {
  return in_ascii_range(c, 48, 57);
}

// Returns true if the token is used inside any of the operator tokens. Note 
// that this includes = and the + inside ++. 
bool is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

// The guidelines are to only take lowercase letters.
bool is_letter(char c) {
  return in_ascii_range(c, 65, 90) || in_ascii_range(c, 97, 122);
}

bool is_keyword(std::string s) {
  return equals_str(s, "FOR") || equals_str(s, "BEGIN") || 
    equals_str(s, "END");
}

bool is_delimiter(char c) {
  return c == '(' || c == ')' || c == ';' || c == ',';
}

void jump(std::string &s, int size) {
  s = s.substr(size);
}

Token begin("\\BOF", "begin_of_file");
Token end("\\EOF", "end_of_file");

// Called when to_parse[0] is numeric
void handle_number(std::string &to_parse, std::vector<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  while (to_parse.size() > 0) { // Append more numbers as necessary
    curr = to_parse[0];
    if (is_numeric(curr)) {
      text = text + curr;
      jump(to_parse, 1);
    } else { // No more numbers to gobble up
      break;
    }
  }
  tokens.push_back(Constant(text));
}

// Possible issue: This doesn't really do anything if there are two operators 
// not separated by whitespace that should be. For instance, this doesn't 
// really care about +-
void handle_operator(std::string &to_parse, std::vector<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  if (curr == '+' && to_parse[0] == '+') {
    text = text + '+';
    jump(to_parse, 1);
    tokens.push_back(SelfOperator(text));
  } else {
    tokens.push_back(BinaryOperator(text));
  }
}

// Called when to_parse[0] is a letter
void handle_letter(std::string &to_parse, std::vector<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  while (to_parse.size() > 0) { // Append more letters as necessary
    curr = to_parse[0];
    if (is_letter(curr)) {
      text += curr;
      jump(to_parse, 1);
    } else {
      break;
    }
  }
  if (is_keyword(text)) {
    tokens.push_back(Keyword(text));
  } else {
    tokens.push_back(Identifier(text));
  }
}

void handle_delimiter(std::string &to_parse, std::vector<Token> &tokens) {
  char curr = to_parse[0];
  std::string text(1, curr);
  jump(to_parse, 1);
  tokens.push_back(Delimiter(text));
}

void tokenize_line(std::string &to_parse, std::vector<Token> &tokens) {
  if (to_parse.size() == 0) {
    return;
  }
  char curr = to_parse[0];
  if (is_numeric(curr)) { // Numbers
    handle_number(to_parse, tokens);
    return tokenize_line(to_parse, tokens);
  } else if (is_operator(curr)) { // Operators
    handle_operator(to_parse, tokens);
    return tokenize_line(to_parse, tokens);
  } else if (is_letter(curr)) { // Letters
    handle_letter(to_parse, tokens);
    return tokenize_line(to_parse, tokens);
  } else if (is_delimiter(curr)) {
    handle_delimiter(to_parse, tokens);
    return tokenize_line(to_parse, tokens);
  } else if (is_space(curr)) { // Spaces
    jump(to_parse, 1);
    return tokenize_line(to_parse, tokens);
  } else {
    std::cerr << "Encountered unknown token: " << curr << std::endl;
    jump(to_parse, 1);
    return tokenize_line(to_parse, tokens);
  }
}

std::string read_line(std::ifstream &ifs) {
  char line[256];
  ifs.getline(line, 256);
  return std::string(line);
}

bool text_is(std::vector<Token> &tokens, int index, std::string to_compare) {
  return equals_str(tokens.at(index).text, to_compare);
}

bool type_is(std::vector<Token> &tokens, int index, std::string to_compare) {
  return equals_str(tokens.at(index).type, to_compare);
}

bool is_for_declarataion(LineWalker &line_walker) {
  return text_is(line_walker.tokens, line_walker.index, "FOR");
}

bool is_identifier(std::vector<Token> &tokens, int index) {
  return type_is(tokens, index, "identifier");
}

// Puts the token in as if it was always there. Such beauty, such grace!
void handle_missing_token(LineWalker &line_walker, Token missing) {
  line_walker.missing.push_back(missing);
  std::vector<Token>::iterator it;
  it = line_walker.tokens.begin();
  std::advance(it, line_walker.index);
  line_walker.tokens.insert(it, missing);
}

void expect_left_paren(LineWalker &line_walker) {
  if (!text_is(line_walker.tokens, line_walker.index, "(")) {
    handle_missing_token(line_walker, Delimiter("("));
  }
  line_walker.index++;
}

void expect_right_paren(LineWalker &line_walker) {
  if (!text_is(line_walker.tokens, line_walker.index, ")")) {
    handle_missing_token(line_walker, Delimiter(")"));
  }
  line_walker.index++; 
}

void expect_comma(LineWalker &line_walker) {
  if (!text_is(line_walker.tokens, line_walker.index, ",")) {
    handle_missing_token(line_walker, Delimiter(","));
  }
  line_walker.index++;
}

void expect_identifier(LineWalker &line_walker) {
  if (!type_is(line_walker.tokens, line_walker.index, "identifier")) {
    handle_missing_token(line_walker, Identifier("_unknown_"));
  }
  line_walker.index++;
}

void expect_constant(LineWalker &line_walker) {
  if (!type_is(line_walker.tokens, line_walker.index, "constant")) {
    handle_missing_token(line_walker, Constant("_unknown_"));
  }
  line_walker.index++;
}

void expect_operator(LineWalker &line_walker) {
  if (!type_is(line_walker.tokens, line_walker.index, "binary_operator") &&
    !type_is(line_walker.tokens, line_walker.index, "self_operator")) {
    handle_missing_token(line_walker, SelfOperator("_unknown_"));
  }
  line_walker.index++;
}

void parse_for_declaration(LineWalker &line_walker) {
  line_walker.index++;
  expect_left_paren(line_walker);
  expect_identifier(line_walker);
  expect_comma(line_walker);
  expect_constant(line_walker);
  expect_comma(line_walker);
  expect_operator(line_walker);
  expect_right_paren(line_walker);
}

void parse_line(LineWalker &line_walker) {
  if (line_walker.tokens.size() == 0) {
    return;
  }
  if (is_for_declarataion(line_walker)) {
    parse_for_declaration(line_walker);
  }
}

CodeBlock tokenize_input(std::ifstream &ifs) {
  CodeBlock program;
  while (!ifs.eof()) { // While there are more lines
    Line line;
    std::string to_parse = read_line(ifs);
    tokenize_line(to_parse, line.tokens);
    program.add_line(line);
  }
  return program;
}

int main(int argc, char **argv) {
  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  CodeBlock program = tokenize_input(ifs);
  ProgramWalker pw;
  for (size_t i = 0; i < program.lines.size(); i++) {
    LineWalker lw(program.lines.at(i).tokens);
    parse_line(lw);
    pw.add_line(lw);
    /*for (size_t j = 0; j < lw.missing.size(); j++) {
      std::cout << "Missing " << lw.missing.at(j).text << " of type " << 
        lw.missing.at(j).type << std::endl;
    }*/
  }
  return 0;
}