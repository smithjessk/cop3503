#ifndef PA1_HEADER
#define PA1_HEADER

#include <vector>

class MagicSquare {
 private:
  int length;
  int magicConstant;
  std::vector<std::vector<int> > grid;

  int findMagicConstant();

 public:
  MagicSquare(int length);

  bool isValidSize();

  void build();

  void test();
};

#endif // PA1_HEADER