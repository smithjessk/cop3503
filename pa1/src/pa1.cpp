#include <iostream>
#include <math.h>

#include "pa1.h"

MagicSquare::MagicSquare(int inputtedLength) {
  this->length = inputtedLength;
  if (isValidSize()) {
    grid.resize(length);
    for (size_t i = 0; i < length; i++) {
      grid[i].resize(length);
    }
  }
}

int MagicSquare::rowSum(int row) {
  int sum = 0;
  std::vector<int> entries = grid[row];
  for (size_t col = 0; col < length; col++) {
    sum += grid[row][col];
  }
  return sum;
}

int MagicSquare::colSum(int col) {
  int sum = 0;
  for (size_t row = 0; row < length; row++) {
    sum += grid[row][col];
  }
  return sum;
}

int MagicSquare::leftDiagSum() {
  int sum = 0;
  for (size_t row = 0; row < length; row++) {
    sum += grid[row][row];
  }
  return sum;
}

int MagicSquare::rightDiagSum() {
  int sum = 0;
  for (size_t row = 0; row < length; row++) {
    sum += grid[row][length - row - 1];
  }
  return sum;
}

bool MagicSquare::isValidSize() {
  bool isOdd = (length % 2 == 1);
  bool isInRange = (length >= 3) && (length <= 15);
  return isOdd && isInRange;
}

/**
 * The magic constant is the value to which every row, column, and diagonal
 * sums to.
 * Source: http://www.geeksforgeeks.org/magic-square/
 */
void MagicSquare::build() {
  // int magicConstant = length * (1 + pow(length, 2)) / 2;
  for (size_t i = 0; i < length; i++) {
    for (size_t j = 0; j < length; j++) {
      grid[i][j] = -1;
    }
  }
  int row = 0;
  int col = length / 2;
  int numSet = 0;
  while (numSet < length * length) {
    while (grid[row][col] != -1) {
      row = (row + 1) % length;
    }
    std::cout << "row: " << row << ", col: " << col << std::endl;
    std::cout << "size: " << grid[row].size() << std::endl;
    std::cout << "val: " << grid[row][col] << std::endl;
    grid[row][col] = numSet + 1;
    std::cout << "val: " << grid[row][col] << std::endl;
    numSet++;
    row = (row - 1) % length;
    col = (col + 1) % length;
    std::cout << "row: " << row << ", col: " << col << std::endl;
  }
  std::cout << "Build square" << std::endl;
}

void MagicSquare::test() {
  std::cout << "Checking the sums of every row: ";
  for (size_t i = 0; i < length; i++) {
    std::cout << rowSum(i) << " ";
  }
  std::cout << "\nChecking the sums of every column: ";
  for (size_t i = 0; i < length; i++) {
    std::cout << colSum(i) << " ";
  }
  std::cout << "\nChecking the sums of every diagonal: ";
  std::cout << leftDiagSum() << " " << rightDiagSum() << std::endl;
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