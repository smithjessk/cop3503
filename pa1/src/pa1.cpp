#include <iostream>
#include <string>
#include <math.h>

#include "pa1.h"

/**
 * The magic constant is the value to which every row, column, and diagonal
 * sums to.
 * Source: http://www.geeksforgeeks.org/magic-square/
 * @param  size The side length of the magic square
 * @return      The magic constant, which is (size) * (1 + size^2) / 2;
 */
int MagicSquare::findMagicConstant() {
  return length * (1 + pow(length, 2)) / 2;
}

MagicSquare::MagicSquare(int inputtedLength) {
  length = inputtedLength;
  if (isValidSize()) {
    magicConstant = findMagicConstant();
    grid.resize(length);
    for (size_t i = 0; i < length; i++) {

    }
  }
}

bool MagicSquare::isValidSize() {
  bool isOdd = (length % 2 == 1);
  bool isInRange = (length >= 3) && (length <= 15);
  return isOdd && isInRange;
}

int promptForSideLength() {
  std::cout << "Enter the side length of a magic square: ";
  int length;
  std::cin >> length;
  return length;
}

int main() {
  int length = promptForSideLength();
  MagicSquare square(length);
  if (!square.isValidSize()) {
    std::string message = "You entered an invalid magic square size.\n" 
      "Size must be an odd integer between 3 and 15, inclusive.\n";
    std::cout << message;
    return 1;
  } else {
    square.build();
    square.test();
    return 0;
  }
}