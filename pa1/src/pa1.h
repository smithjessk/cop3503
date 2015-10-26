#ifndef PA1_HEADER
#define PA1_HEADER

#include <string>
#include <vector>

/**
 * Represents a square grid of size n by n. The grid contains the entries 1 
 * through n^2, and the entries in each row, column, and main diagonal sum to
 * the same value. 
 */
class MagicSquare {
 private:
  // Side length of the grid. 
  int length;

  // Each vector represents a row. The elements in that row are contained
  // within that vector.
  std::vector<std::vector<int> > grid;

  // Resize grid to length * length, setting each entry to -1. 
  // Used in the constructor if the inputted length was valid.
  void resizeGrid();

  /** 
   * Calculate the sum of the elements of the passed row. Note that the row
   * indices start at 0 and continue in the same manner as normal vector 
   * indices.
   */
  int rowSum(int row);

  /**
   * Calculate the sum of the elements of the passed column. Note that the
   * column indices start at 0 and continue in the same manner as normal 
   * vector indices.
   */
  int colSum(int col);

  /**
   * Calculate the sum of the grid entries along the main "upper-left"
   * diagonal. That is, the entries (0, 0), (2, 2), and so on until 
   * (length - 1, length - 1).
   */
  int leftDiagSum();

  /**
   * Calculate the sum of the grid entries along the main "upper-right"
   * diagonal. That is, the entries (length - 1, 0), (length - 2, 1), and so 
   * on until (0, length - 1). Note that (i, j) corresponds to the i-th row
   * and j-th column.
   */
  int rightDiagSum();

 public:
  /**
   * Take an inputted length and, if appropriate, as determined by 
   * isValidSize(), then call resizeGrid().
   */
  MagicSquare(int length);

  /**
   * Check if the matrix is a valid size. Do so before the grid is actually
   * created in order to prevent the allocation of, in the worst case, a 
   * million by million matrix.
   */
  bool isValidSize();

  /**
   * Create the matrix. Namely, follow the algorithm outlined in 
   * http://mathworld.wolfram.com/MagicSquare.html
   */
  void build();

  // Print out the sum of each row, column, and diagonal. Return whether or 
  // not all of the sums are the same. 
  bool test();
};

#endif // PA1_HEADER