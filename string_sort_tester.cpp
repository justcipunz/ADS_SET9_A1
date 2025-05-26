#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <utility>

class StringSortTester {
 public:
  enum SortType {
    STANDARD_MERGESORT,
    STANDARD_QUICKSORT,
    TERNARY_QUICKSORT,
    LCP_MERGESORT,
    MSD_RADIXSORT,
    MSD_RADIXSORT_WITH_QUICKSORT
  };

  struct Result {
    long long comparisons = 0;
    double elapsed_ms = 0.0;
  };

  Result test(SortType type, std::vector<std::string> input) {
    long long cmp = 0;
    auto start = std::chrono::high_resolution_clock::now();

    switch (type) {
      case STANDARD_MERGESORT:
        mergeSort(input, 0, input.size() - 1, cmp);
        break;
      case STANDARD_QUICKSORT:
        quickSort(input, 0, input.size() - 1, cmp);
        break;
      case TERNARY_QUICKSORT:
        ternary_string_quicksort(input, 0, input.size() - 1, 0, cmp);
        break;
      case LCP_MERGESORT: {
        std::vector<size_t> lcp(input.size());
        string_merge_sort(input, lcp, 0, input.size() - 1, cmp);
        break;
      }
      case MSD_RADIXSORT:
        msd_radix_sort(input, 0, input.size() - 1, 0, cmp, false);
        break;
      case MSD_RADIXSORT_WITH_QUICKSORT:
        msd_radix_sort(input, 0, input.size() - 1, 0, cmp, true);
        break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {cmp, ms};
  }

  void write_csv_header(std::ofstream& out) {
    out << "Size,Algorithm,DataType,Comparisons,Time_ms\n";
  }

  void write_csv_row(std::ofstream& out, int size, const std::string& algo, const std::string& dtype,
                     const Result& res) {
    out << size << "," << algo << "," << dtype << "," << res.comparisons << "," << std::fixed << std::setprecision(3)
        << res.elapsed_ms << "\n";
  }

 private:
  // ===== Стандартный MergeSort =====
  int compare_strings(const std::string& a, const std::string& b, long long& cmp) {
    size_t min_len = std::min(a.length(), b.length());
    for (size_t i = 0; i < min_len; ++i) {
      ++cmp;
      if (a[i] != b[i])
        return a[i] < b[i] ? -1 : 1;
    }
    ++cmp;
    if (a.length() == b.length())
      return 0;
    return a.length() < b.length() ? -1 : 1;
  }

  void merge(std::vector<std::string>& arr, int l, int m, int r, long long& cmp) {
    std::vector<std::string> temp(r - l + 1);
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
      if (compare_strings(arr[i], arr[j], cmp) <= 0)
        temp[k++] = arr[i++];
      else
        temp[k++] = arr[j++];
    }
    while (i <= m)
      temp[k++] = arr[i++];
    while (j <= r)
      temp[k++] = arr[j++];
    for (int p = 0; p < k; ++p)
      arr[l + p] = std::move(temp[p]);
  }

  void mergeSort(std::vector<std::string>& arr, int l, int r, long long& cmp) {
    if (l < r) {
      int m = (l + r) / 2;
      mergeSort(arr, l, m, cmp);
      mergeSort(arr, m + 1, r, cmp);
      merge(arr, l, m, r, cmp);
    }
  }

  // ===== Стандартный QuickSort =====
  int partition(std::vector<std::string>& arr, int low, int high, long long& cmp) {
    const std::string& pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
      if (compare_strings(arr[j], pivot, cmp) < 0) {
        ++i;
        std::swap(arr[i], arr[j]);
      }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
  }

  void quickSort(std::vector<std::string>& arr, int low, int high, long long& cmp) {
    if (low < high) {
      int pi = partition(arr, low, high, cmp);
      quickSort(arr, low, pi - 1, cmp);
      quickSort(arr, pi + 1, high, cmp);
    }
  }

  // ===== Тернарный STRING_QUICKSORT =====
  char get_char(const std::string& s, int pos) {
    return pos < s.length() ? s[pos] : 0;
  }

  void ternary_string_quicksort(std::vector<std::string>& arr, int left, int right, int char_index,
                                long long& comparisons) {
    if (left >= right)
      return;

    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(left, right);
    std::swap(arr[left], arr[dist(rng)]);

    char pivot_char = get_char(arr[left], char_index);
    int less_than = left, greater_than = right, current = left + 1;

    while (current <= greater_than) {
      char current_char = get_char(arr[current], char_index);
      ++comparisons;

      if (current_char < pivot_char) {
        std::swap(arr[less_than++], arr[current++]);
      } else if (current_char > pivot_char) {
        std::swap(arr[current], arr[greater_than--]);
      } else {
        ++current;
      }
    }

    ternary_string_quicksort(arr, left, less_than - 1, char_index, comparisons);
    if (pivot_char != 0)
      ternary_string_quicksort(arr, less_than, greater_than, char_index + 1, comparisons);
    ternary_string_quicksort(arr, greater_than + 1, right, char_index, comparisons);
  }

  // ===== STRING MERGESORT with LCP =====
  struct LCPResult {
    int cmp_result;
    size_t lcp_length;
  };

  LCPResult lcp_compare(const std::string& a, const std::string& b, size_t known_lcp, long long& comparisons) {
    size_t i = known_lcp;
    while (i < a.length() && i < b.length()) {
      ++comparisons;
      if (a[i] != b[i]) {
        return {a[i] < b[i] ? -1 : 1, i};
      }
      ++i;
    }

    if (a.length() == b.length()) {
      return {0, i};
    }
    return {a.length() < b.length() ? -1 : 1, i};
  }

  void string_merge_sort(std::vector<std::string>& arr, std::vector<size_t>& lcp, int l, int r,
                         long long& comparisons) {
    if (l >= r)
      return;

    int m = l + (r - l) / 2;
    string_merge_sort(arr, lcp, l, m, comparisons);
    string_merge_sort(arr, lcp, m + 1, r, comparisons);

    std::vector<std::string> temp(r - l + 1);
    int i = l, j = m + 1, k = 0;

    while (i <= m && j <= r) {
      LCPResult cmp = lcp_compare(arr[i], arr[j], 0, comparisons);
      if (cmp.cmp_result <= 0) {
        temp[k++] = arr[i++];
      } else {
        temp[k++] = arr[j++];
      }
    }

    while (i <= m)
      temp[k++] = arr[i++];
    while (j <= r)
      temp[k++] = arr[j++];

    for (int p = 0; p < k; ++p) {
      arr[l + p] = std::move(temp[p]);
    }
  }

  // ===== MSD_RADIX_SORT =====
  void msd_radix_sort(std::vector<std::string>& arr, int left, int right, int char_index, long long& comparisons,
                      bool use_quicksort_cutoff = false) {
    if (left >= right)
      return;

    if (use_quicksort_cutoff && (right - left + 1) <= 74) {
      ternary_string_quicksort(arr, left, right, char_index, comparisons);
      return;
    }

    const int R = 256;
    std::vector<int> count(R + 2, 0);
    std::vector<std::string> aux(right - left + 1);

    for (int i = left; i <= right; ++i) {
      char c = get_char(arr[i], char_index);
      count[c + 2]++;
      if (char_index < arr[i].length())
        ++comparisons;
    }

    for (int r = 0; r < R + 1; ++r) {
      count[r + 1] += count[r];
    }

    for (int i = left; i <= right; ++i) {
      char c = get_char(arr[i], char_index);
      aux[count[c + 1]++] = arr[i];
    }

    for (int i = left; i <= right; ++i) {
      arr[i] = aux[i - left];
    }

    for (int r = 0; r < R; ++r) {
      int start = left + count[r];
      int end = left + count[r + 1] - 1;
      if (start < end) {
        msd_radix_sort(arr, start, end, char_index + 1, comparisons, use_quicksort_cutoff);
      }
    }
  }
};
