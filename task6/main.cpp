#include <algorithm>
#include <iostream>

bool cmp(int a, int b) {
  if (a < b) {
    return true;
  }

  return false;
}

int median_of_three(int left, int mid, int right) {
  if ((left < mid && mid < right) || (right < mid && mid < left)) {
    return mid;
  }

  if ((mid < left && left < right) || (right < left && left < mid)) {
    return left;
  }

  return right;
}

int partition(int* arr, int left, int right, bool (*cmp)(int, int)) {
  int pivot = median_of_three(left, (right - left) / 2, right);

  if(pivot != right-1) {
    std::swap(arr[pivot], arr[right - 1]);
    pivot = right - 1;
  }

  int j = pivot - 1;
  int i = pivot - 1;

  while(i >= left && j >= left) {
    if (i != j && !cmp(arr[i], arr[pivot])) {
      std::swap(arr[i], arr[j]);
    }
    
    if (!cmp(arr[j], arr[pivot])) {
      j--;
    }
    i--;
  }
  
  std::swap(arr[j+1], arr[pivot]);

  return j+1;
}

int find_kth(int* arr, int n, int k, bool (*cmp)(int, int)) {
  int left = 0, right = n;

  while(true) {
    int mid = partition(arr, left, right, cmp);
    if (mid == k) {
      return arr[mid];
    } else if (k < mid) {
      right = mid;
    } else {
      left = mid + 1;
    }
  }
}

void run(std::istream& input, std::ostream& output) {
  int n = 0;
  input >> n;
  
  int k = 0;
  input >> k;

  int arr[n];

  int value = 0;
  for(int i = 0; i < n; i++) {
    input >> value;
    arr[i] = value;
  }
  
  value = find_kth(arr, n, k, cmp);

  output << value;
}

int main() { 
  run(std::cin, std::cout);

  return 0;
}
