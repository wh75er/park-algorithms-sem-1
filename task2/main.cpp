/*
 * Задача 2_1.
 *
 *    Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. 
 * Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, 
 * равного или превосходящего B[i]: A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
 *
 *    Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче 
 * для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k, 
 * а потом уже в нем делать бинарный поиск.
*/

#include <iostream>
#include <chrono>

void find_elements(const int n, const int* const a, const int m, const int* const b, int* const size, int* const result);
int exp_search(const int size, const int* const arr, const int x);
int bin_search(const int size, const int* const arr, const int b_i, const int l_boundary, const int r_boundary);
void run(std::istream& in, std::ostream& out);

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  run(std::cin, std::cout);
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;

  std::cout << "All Time is: " << elapsed.count() << std::endl;

  return 0;
}

void run(std::istream& in, std::ostream& out) {
  int n = 0, m = 0;

  in >> n;
  in >> m;

  int* a = new int[n];
  int* b = new int[m];

  for(int i = 0; i < n; i++) {
    in >> a[i];
  }

  for(int i = 0; i < m; i++) {
    in >> b[i];
  }

  int size = 0;
  int* result = new int[m];
  auto start = std::chrono::high_resolution_clock::now();
  find_elements(n, a, m, b, &size, result);
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;

  std::cout << "Time is: " << elapsed.count() << std::endl;

  for(int i = 0; i < size; i++) {
    out << result[i] << " ";
  }
  out << std::endl;

  delete [] a;
  delete [] b;
  delete [] result;
}

void find_elements(const int n, const int* const a, const int m, const int* const b, int* const size, int* const result) {
  *size = 0;

  for(int i = 0; i < m; i++) {
    std::cout << std::endl;
    int r_boundary = exp_search(n, a, b[i]);
    int index = bin_search(n, a, b[i], r_boundary/2, r_boundary);

    result[*size] = index;
    (*size)++;
  }
}

int exp_search(const int size, const int* const arr, const int x) {
  int i = 1;
  while(i < size && arr[i] < x) {
    i *= 2;
  }

  return i;
}

int bin_search(const int size, const int* const arr, const int b_i, const int l_boundary, const int r_boundary) {
  int left = l_boundary;
  int right = r_boundary;

  if(right > size) {
    right = size-1;
  }

  while(left < right) {
    int mid = left + ((right - left) / 2);
    if(arr[mid] >= b_i) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }

  return arr[left] >= b_i ? left : size;
}
