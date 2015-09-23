#ifndef PA1_HEADER
#define PA1_HEADER

#include <string>
#include <vector>

class MagicSquare {
 private:
  int length;

  // Outside vectors are rows, inside ones are elements in that row
  std::vector<std::vector<int> > grid;

  void resizeGrid();

  int rowSum(int row);

  int colSum(int col);

  int leftDiagSum();

  int rightDiagSum();

 public:
  MagicSquare(int length);

  std::string toString();

  bool isValidSize();

  void build();

  void test();
};

#endif // PA1_HEADER