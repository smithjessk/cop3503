#include <iostream>
#include <math.h>
#include <cstdio>

#include "pa1.h"

/**
 * Find the positive integer n such that n = number (mod) modulo.
 * Similar functionality to number % modulo, except this always ensures a 
 * positive result.
 * @param  number The number to take the mod of
 * @param  modulo The number by which we divide
 * @return        The number n as described above.
 */
int takePositiveMod(int number, int modulo) {
  int result = number % modulo;
  if (result < 0) {
    result += modulo;
  }
  return result;
}

MagicSquare::MagicSquare(int inputtedLength) {
  length = inputtedLength;
  if (isValidSize()) {
    resizeGrid();  
  }
}

void MagicSquare::resizeGrid() {
  grid.resize(length);
  for (int i = 0; i < length; i++) {
    grid[i].resize(length);
  }
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      grid[i][j] = -1;
    }
  }
}

int MagicSquare::rowSum(int row) {
  int sum = 0;
  std::vector<int> entries = grid[row];
  for (int col = 0; col < length; col++) {
    sum += grid[row][col];
  }
  return sum;
}

int MagicSquare::colSum(int col) {
  int sum = 0;
  for (int row = 0; row < length; row++) {
    sum += grid[row][col];
  }
  return sum;
}

int MagicSquare::leftDiagSum() {
  int sum = 0;
  for (int row = 0; row < length; row++) {
    sum += grid[row][row];
  }
  return sum;
}

int MagicSquare::rightDiagSum() {
  int sum = 0;
  for (int row = 0; row < length; row++) {
    sum += grid[row][length - row - 1];
  }
  return sum;
}

/**
 * Determine if the length field on this MagicSquare instance is valid. Namely,
 * assert that it must be odd and between the numbers 3 and 15, inclusive.
 * @return Whether or not size fits the above descriptions.
 */
bool MagicSquare::isValidSize() {
  bool isOdd = (length % 2 == 1);
  bool isInRange = (length >= 3) && (length <= 15);
  return isOdd && isInRange;
}

/**
 * 
 * Source: http://mathworld.wolfram.com/MagicSquare.html
 */
void MagicSquare::build() {
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

/**
 * Check and print the sum of each row, column, and major diagonal in the grid.
 * @return Whether or not the each value currentSum took on was the same as the
 * value of acceptedSum.
 */
bool MagicSquare::test() {
  int acceptedSum = rowSum(0);
  int currentSum;
  bool consistentSums = true;
  std::cout << "Checking the sums of every row: ";
  for (int i = 0; i < length; i++) {
    currentSum = rowSum(i);
    std::cout << currentSum << " ";
    if (currentSum != acceptedSum) {
      consistentSums = false;
    }
  }
  std::cout << "\nChecking the sums of every column: ";
  for (int i = 0; i < length; i++) {
    currentSum = colSum(i);
    std::cout << currentSum << " ";
    if (currentSum != acceptedSum) {
      consistentSums = false;
    }
  }
  std::cout << "\nChecking the sums of every diagonal: ";
  std::cout << leftDiagSum() << " " << rightDiagSum() << std::endl;
  if (leftDiagSum() != acceptedSum || rightDiagSum() != acceptedSum) {
    consistentSums = false;
  }
  return consistentSums;
}

// Prompt for and return what the user wants the side length of the magic
// square to be. 
int promptForSideLength() {
  std::cout << "Enter the side length of a magic square: ";
  int length;
  std::cin >> length;
  return length;
}

/**
 * Take in a side length. If it is valid, build and test a magic square with
 * that side length. If the magic square does not pass the test() method, then 
 * return 1.
 * If the size was invalid, print out an appropriate error message and return 1
 * to denote that something went wrong.
 */
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
    bool buildPassed = square.test();
    if (!buildPassed) {
      std::cout << "The square could not be correctly built." << std::endl;
      return 1;
    } else {
      std::cout << "Build successful." << std::endl;
      return 0;
    }
  }
}