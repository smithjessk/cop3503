#include <iostream>

int promptForSize() {
  std::cout << "Enter the size of a magic square: ";
  int size;
  std::cin >> size;
  std::cout << std::endl;
  return size;
}

bool checkSize(int size) {
  bool isOdd = (size % 2 == 1);
  bool isInRange = (size >= 3 && size <= 15);
  return isOdd && isInRange;
}

int main() {
  int size = promptForSize();
  bool validSize = checkSize(size);
  if (!validSize) {
    std::cout << "You entered an invalid magic square size." << std::endl <<
      "Size must be an odd integer between 3 and 15, inclusive." << std::endl;
    return 1;
  } else {
    // MagicSquare square;
    // square = buildMagicSquare(size);
    // testMagicSquare();
    return 0;
  }
}