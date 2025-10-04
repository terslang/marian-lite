#pragma once

// This is a shallow wrapper around a filesystem path library.
// We used this to wrap boost::filesystem, now we are wrapping
// Pathie, a small open source lib.

// 2025 October: Replaced pathie with std::filesystem
// kept the wrapper so as to minimize code changes
// - s20n

// Even when compiling with clang, __GNUC__ may be defined, so
// we need to add some extra checks to avoid compile errors with
// respect to -Wsuggest-override.
#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-value"
#  if defined(__has_warning)
#    if __has_warning("-Wsuggest-override")
#      pragma GCC diagnostic ignored "-Wsuggest-override"
#    endif
#  else
#    pragma GCC diagnostic ignored "-Wsuggest-override"
#  endif
#endif

#include <filesystem>
#include <string>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

namespace marian {
namespace filesystem {

  bool is_fifo(char const* path);
  bool is_fifo(std::string const& path);

  class Path {
    private:
      std::filesystem::path path;

    public:
      Path() {}
      Path(const Path& p) : path{p.path} {}
      Path& operator=(const Path& p) = default;
      Path(const std::string& s) : path{s} {}
      Path(const std::filesystem::path& p) : path{p} {}

      Path parentPath() const {
        return Path(path.parent_path());
      }

      Path filename() const {
        return Path(path.filename());
      }

      Path extension() const {
        return Path(path.extension());
      }

      bool empty() const {
        return path.empty();
      }

      const std::filesystem::path& getImpl() const {
        return path;
      }

      operator std::string() const {
        return path.string();
      }

      std::string string() const {
        return path.string();
      }

      bool operator==(const Path& p) const {
        return path == p.path;
      }

      bool operator!=(const Path& p) const {
        return path != p.path;
      }
  };

  static inline Path currentPath() {
    return Path(std::filesystem::current_path());
  }

  static inline Path canonical(const Path& p, const Path& base) {
    return Path(std::filesystem::canonical(base.getImpl() / p.getImpl()));
  }

  static inline Path relative(const Path& p, const Path& base) {
    return Path(std::filesystem::relative(p.getImpl(), base.getImpl()));
  }

  static inline bool exists(const Path& p) {
    return std::filesystem::exists(p.getImpl());
  }

  static inline size_t fileSize(const Path& p) {
    return std::filesystem::file_size(p.getImpl());
  }

  static inline bool isDirectory(const Path& p) {
    return std::filesystem::is_directory(p.getImpl());
  }

  static inline Path operator/ (const Path& lhs, const Path& rhs) {
    return Path(lhs.getImpl() / rhs.getImpl());
  }

  using FilesystemError = std::filesystem::filesystem_error;

}  // namespace filesystem
}  // namespace marian
