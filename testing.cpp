#include "string_generator.cpp"
#include "string_sort_tester.cpp"

int main() {
  StringGenerator generator;
  StringSortTester tester;

  std::ofstream out("results.csv");
  tester.write_csv_header(out);

  std::vector<StringSortTester::SortType> types = {
      StringSortTester::STANDARD_MERGESORT, StringSortTester::STANDARD_QUICKSORT,
      StringSortTester::TERNARY_QUICKSORT,  StringSortTester::LCP_MERGESORT,
      StringSortTester::MSD_RADIXSORT,      StringSortTester::MSD_RADIXSORT_WITH_QUICKSORT};

  std::vector<std::string> names = {"StandardMergeSort", "StandardQuickSort", "TernaryQuickSort",
                                    "LCPMergeSort",      "MSDRadixSort",      "MSDRadixSort+QuickSort"};

  for (int size = 100; size <= 3000; size += 100) {
    std::vector<std::pair<std::string, std::vector<std::string>>> datasets = {
        {"Random", generator.generate_random_strings(size)},
        {"Reversed", generator.generate_reversed_strings(size)},
        {"AlmostSorted", generator.generate_almost_sorted_strings(size)},
        {"Prefix", generator.generate_prefix_strings(size, "prefix_")}};

    for (size_t i = 0; i < types.size(); ++i) {
      for (const auto& [dtype, data] : datasets) {
        auto res = tester.test(types[i], data);
        tester.write_csv_row(out, size, names[i], dtype, res);
      }
    }
  }

  out.close();
  std::cout << "Analysis complete. See 'results.csv'\n";
  return 0;
}
