#include <iostream>
#include <vector>
#include <string>

using namespace std;

void radix_sort(vector<string> &arr, int depth = 0) {
  if (arr.size() <= 1)
    return;

  const int ALPHABET_SIZE = 256;
  vector<vector<string>> buckets(ALPHABET_SIZE + 1);

  for (const auto &s : arr) {
    if (depth < s.length()) {
      buckets[static_cast<unsigned char>(s[depth]) + 1].push_back(s);
    } else {
      buckets[0].push_back(s);
    }
  }

  arr.clear();
  for (int i = 0; i <= ALPHABET_SIZE; ++i) {
    if (!buckets[i].empty()) {
      if (i > 0)
        radix_sort(buckets[i], depth + 1);
      arr.insert(arr.end(), buckets[i].begin(), buckets[i].end());
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  string count_str;
  getline(cin, count_str);
  int n = std::stoi(count_str);

  vector<string> data(n);
  for (int i = 0; i < n; ++i) {
    getline(cin, data[i]);
  }

  radix_sort(data);

  for (const auto &line : data) {
    cout << line << '\n';
  }

  return 0;
}
