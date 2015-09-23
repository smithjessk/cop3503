#include <iostream>
#include <math.h>
#include <cstdio>

#include "pa1.h"

/**
 * Similar functionality to number % modulo, except this always ensures a 
 * positive result.
 * @param  number The number to take the mod of
 * @param  modulo The number by which we divide
 * @return        The positive integer n such that n = number (mod) modulo.
 */
int takePositiveMod(int number, int modulo) {
  int result = number % modulo;
  if (result < 0) {
    result += modulo;
  }
  return result;
}

MagicSquare::MagicSquare(int inputtedLength) {
  this->length = inputtedLength;
  if (isValidSize()) {
    grid.resize(length);
    for (size_t i = 0; i < length; i++) {
      grid[i].resize(length);
    }
  }
}

void MagicSquare::resizeGrid() {
  for (size_t i = 0; i < length; i++) {
    for (size_t j = 0; j < length; j++) {
      grid[i][j] = -1;
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
 * Source: http://mathworld.wolfram.com/MagicSquare.html
 */
void MagicSquare::build() {
  resizeGrid();
  int row = 0;
  int col = length / 2;
  int numSet = 0;
  while (numSet < length * length) {
    grid[row][col] = numSet + 1;
    numSet++;
    int nextRow = takePositiveMod(row - 1, length), 
      nextCol = takePositiveMod(col + 1, length);
    if (grid[nextRow][nextCol] == -1) { 
      row = nextRow;
      col = nextCol;
    } else {
      if (numSet < length * length) {
        while (grid[row][col] != -1) {
          row = takePositiveMod(row + 1, length);
        }
      }
    }
  }
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