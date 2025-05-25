#include <bits/stdc++.h>
using namespace std;

constexpr int ALPHABET_SIZE = 74;
constexpr int BUCKETS = 257;

char charAt(const string &s, int d) {
  return d < s.length() ? s[d] : 0;
}

void quickSortTernary(vector<string> &a, int lo, int hi, int d) {
  if (lo >= hi)
    return;

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(lo, hi);
  int pivotIdx = dis(gen);
  swap(a[lo], a[pivotIdx]);

  char v = charAt(a[lo], d);
  int lt = lo, gt = hi;
  int i = lo + 1;

  while (i <= gt) {
    char t = charAt(a[i], d);
    if (t < v)
      swap(a[lt++], a[i++]);
    else if (t > v)
      swap(a[i], a[gt--]);
    else
      i++;
  }

  quickSortTernary(a, lo, lt - 1, d);
  if (v != 0)
    quickSortTernary(a, lt, gt, d + 1);
  quickSortTernary(a, gt + 1, hi, d);
}

void hybridRadixSort(vector<string> &a, int d = 0) {
  if (a.size() <= 1)
    return;

  if (a.size() < ALPHABET_SIZE) {
    quickSortTernary(a, 0, a.size() - 1, d);
    return;
  }

  vector<vector<string>> bins(BUCKETS);
  for (const auto &s : a) {
    char c = charAt(s, d);
    bins[c + 1].emplace_back(s);
  }

  a.clear();
  for (int i = 0; i < BUCKETS; ++i) {
    if (!bins[i].empty()) {
      if (i > 0)
        hybridRadixSort(bins[i], d + 1);
      a.insert(a.end(), bins[i].begin(), bins[i].end());
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  string s;
  getline(cin, s);
  int n = stoi(s);
  vector<string> data(n);

  for (int i = 0; i < n; ++i)
    getline(cin, data[i]);

  hybridRadixSort(data);

  for (const auto &str : data)
    cout << str << '\n';

  return 0;
}
