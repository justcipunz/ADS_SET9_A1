#include <iostream>
#include <string>
#include <vector>

using namespace std;

int compareWithLCP(const string& a, const string& b, size_t& lcp_out, size_t start = 0) {
  size_t i = start;
  while (i < a.size() && i < b.size() && a[i] == b[i]) {
    ++i;
  }
  lcp_out = i;
  if (i == a.size() || i == b.size()) {
    if (a.size() == b.size())
      return 0;
    return a.size() < b.size() ? -1 : 1;
  }
  return a[i] < b[i] ? -1 : 1;
}

void mergeLCP(const vector<string>& left, const vector<string>& right, vector<string>& merged) {
  size_t l = 0, r = 0;
  merged.clear();

  while (l < left.size() && r < right.size()) {
    size_t lcp_dummy;
    int cmp = compareWithLCP(left[l], right[r], lcp_dummy);
    if (cmp <= 0) {
      merged.push_back(left[l++]);
    } else {
      merged.push_back(right[r++]);
    }
  }

  while (l < left.size()) {
    merged.push_back(left[l++]);
  }
  while (r < right.size()) {
    merged.push_back(right[r++]);
  }
}

void lcpMergeSort(vector<string>& arr) {
  if (arr.size() <= 1)
    return;

  size_t mid = arr.size() / 2;
  vector<string> left(arr.begin(), arr.begin() + mid);
  vector<string> right(arr.begin() + mid, arr.end());

  lcpMergeSort(left);
  lcpMergeSort(right);

  mergeLCP(left, right, arr);
}

// int main() {
//   std::ios_base::sync_with_stdio(false);
//   cin.tie(nullptr);
//   cout.tie(nullptr);

//   int n;
//   cin >> n;
//   cin.ignore();

//   vector<string> lines(n);
//   for (int i = 0; i < n; ++i) {
//     std::getline(cin, lines[i]);
//   }

//   lcpMergeSort(lines);

//   for (const auto& str : lines) {
//     cout << str << '\n';
//   }

//   return 0;
// }
