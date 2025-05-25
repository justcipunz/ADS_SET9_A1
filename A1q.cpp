#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

char symbolAt(const string &s, int index) {
  return (index < static_cast<int>(s.size())) ? s[index] : 0;
}

void quickSortStrings(vector<string> &data, int lo, int hi, int depth) {
  if (lo >= hi)
    return;

  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(lo, hi);
  int pivot_pos = dist(rng);
  std::swap(data[lo], data[pivot_pos]);
  char pivot = symbolAt(data[lo], depth);

  int less = lo, more = hi, current = lo + 1;

  while (current <= more) {
    char c = symbolAt(data[current], depth);
    if (c < pivot) {
      std::swap(data[current], data[less++]);
      ++current;
    } else if (c > pivot) {
      std::swap(data[current], data[more--]);
    } else {
      ++current;
    }
  }

  quickSortStrings(data, lo, less - 1, depth);
  if (pivot != 0) {
    quickSortStrings(data, less, more, depth + 1);
  }
  quickSortStrings(data, more + 1, hi, depth);
}

void performSort(vector<string> &arr) {
  if (!arr.empty()) {
    quickSortStrings(arr, 0, static_cast<int>(arr.size()) - 1, 0);
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int n;
  cin >> n;
  cin.ignore();

  vector<string> lines(n);
  for (int i = 0; i < n; ++i) {
    getline(cin, lines[i]);
  }

  performSort(lines);

  for (const auto &str : lines) {
    cout << str << '\n';
  }

  return 0;
}
