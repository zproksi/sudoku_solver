#pragma once

namespace brute_force
{

constexpr const int N = 9;
constexpr const int UNASSIGNED = 0;

void solve_sudoku(const char in_grid[81], char out_grid[81]);


static int is_exist_row(int grid[N][N], int row, int num){
   for (int col = 0; col < N; col++) {
      if (grid[row][col] == num) {
         return 1;
      }
   }
   return 0;
}

static int is_exist_col(int grid[N][N], int col, int num){
   for (int row = 0; row < N; row++) {
      if (grid[row][col] == num) {
         return 1;
      }
   }
   return 0;
}

static int is_exist_box(int grid[N][N], int startRow, int startCol, int num){
   for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
         if (grid[row + startRow][col + startCol] == num) {
            return 1;
         }
      }
   }
   return 0;
}

static int is_safe_num(int grid[N][N], int row, int col, int num){
   return !is_exist_row(grid, row, num) && !is_exist_col(grid, col, num) && !is_exist_box(grid, row - (row % 3), col - (col % 3), num);
}

static int find_unassigned(int grid[N][N], int* row, int* col){
   for (*row = 0; *row < N; (*row)++) {
      for (*col = 0; *col < N; (*col)++) {
         if (grid[*row][*col] == UNASSIGNED) {
            return 1;
         }
      }
   }
   return 0;
}

static int solve(int grid[N][N]){
   int row = 0;
   int col = 0;

   if (!find_unassigned(grid, &row, &col)){
      return 1; // success!
   }

   for (int num = 1; num <= N; num++) {
      if (is_safe_num(grid, row, col, num)) {
         grid[row][col] = num;

         if (solve(grid)) {
            return 1;
         }

         grid[row][col] = UNASSIGNED;
      }
   }
   return 0; // this triggers backtracking
}

void flatten_grid(int full_grid[N][N], char out_grid[81]){
   for (int i = 0; i < N; ++i){
      for (int j = 0; j < N; ++j){
         out_grid[i * N + j] = static_cast<char>('0' + full_grid[i][j]);
      }
   }
}

void unflatten_grid(const char in_grid[81], int full_grid[N][N]){
   for (int i = 0; i < N; ++i){
      for (int j = 0; j < N; ++j){
         const char cc = in_grid[i * N + j];
         if (cc >= '1' && cc <='9')
         {
            full_grid[i][j] = in_grid[i * N + j] - '0';
         }
         else
         {
            full_grid[i][j] = 0;
         }
      }
   }
}

void solve_sudoku(const char in_grid[81], char out_grid[81]){
   int grid[N][N];
   unflatten_grid(in_grid, grid);

   if (solve(grid))
   {
      flatten_grid(grid, out_grid);
   }
}

} // namespac brute_force
