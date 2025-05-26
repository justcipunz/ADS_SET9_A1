#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

class StringGenerator {
 private:
  const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";

  std::mt19937 rng;
  std::uniform_int_distribution<size_t> len_dist;
  std::uniform_int_distribution<size_t> char_dist;

 public:
  StringGenerator()
      : rng(std::chrono::steady_clock::now().time_since_epoch().count())
      , len_dist(10, 200)
      , char_dist(0, chars.size() - 1) {
  }

  std::string generate_string() {
    size_t length = len_dist(rng);
    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
      result += chars[char_dist(rng)];
    return result;
  }

  std::vector<std::string> generate_random_strings(size_t count) {
    std::vector<std::string> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
      result.push_back(generate_string());
    return result;
  }

  std::vector<std::string> generate_reversed_strings(size_t count) {
    auto arr = generate_random_strings(count);
    std::sort(arr.rbegin(), arr.rend());
    return arr;
  }

  std::vector<std::string> generate_almost_sorted_strings(size_t count, int swaps = 10) {
    auto arr = generate_random_strings(count);
    std::sort(arr.begin(), arr.end());
    std::uniform_int_distribution<size_t> dist(0, count - 1);
    for (int i = 0; i < swaps && count > 1; ++i) {
      size_t a = dist(rng), b = dist(rng);
      if (a != b)
        std::swap(arr[a], arr[b]);
    }
    return arr;
  }

  std::vector<std::string> generate_prefix_strings(size_t count, const std::string& prefix = "test") {
    std::vector<std::string> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
      result.push_back(prefix + generate_string());
    return result;
  }
};
