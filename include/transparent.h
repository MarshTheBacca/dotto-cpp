#include <string>
#include <string_view>

/**
 * @brief Custom hasher for std::unordered_set and std::unordered_map
 * that allows for heterogeneous lookup, meaning we can compare different
 * types transparently, meaning no type conversion is needed.
 */
struct TransparentHasher {
  // Enable heterogeneous lookup
  using is_transparent = void;

  std::size_t operator()(const std::string &str) const noexcept {
    return std::hash<std::string>{}(str);
  }

  std::size_t operator()(std::string_view str) const noexcept {
    return std::hash<std::string_view>{}(str);
  }
};

/**
 * @brief Custom equality function for std::unordered_set and std::unordered_map
 * with methods defined to equate std::string and std::string_view.
 */
struct TransparentEqual {
  using is_transparent = void;

  bool operator()(const std::string &lhs,
                  const std::string &rhs) const noexcept {
    return lhs == rhs;
  }

  bool operator()(const std::string &lhs, std::string_view rhs) const noexcept {
    return lhs == rhs;
  }

  bool operator()(std::string_view lhs, const std::string &rhs) const noexcept {
    return lhs == rhs;
  }

  bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
    return lhs == rhs;
  }
};