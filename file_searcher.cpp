/**
 *
 * file_system.cpp
 *
 */

#include "file_searcher.h"
#include "constants.h"
#include "utils.h"
#include <fstream>

namespace fs = boost::filesystem;

FileSearcher::FileSearcher(const std::string &needle)
    : m_needle(needle), m_boyer_moore(needle.begin(), needle.end()) {}

FileSearcher::FileSearcher(const std::string &needle, const fs::path &path,
                           std::ostream &out)
    : FileSearcher(needle) {
  operator()(path, out);
}

void FileSearcher::operator()(const fs::path &path, std::ostream &out) const {
  std::string file_name = path.filename().string();
  trim_quotation_marks(file_name);
  std::ifstream in(path.string());
  if (!in.is_open()) {
    std::cout << "Could not open file " << file_name << std::endl;
    return;
  }

  std::size_t buff_len = fs::file_size(path);
  if (buff_len < BUF_LEN_MIN) {
    buff_len = BUF_LEN_MIN;
  }
  if (buff_len > BUF_LEN_MAX) {
    buff_len = BUF_LEN_MAX;
  }

  // allocate buffer to read into;
  std::unique_ptr<char[]> buffer(new (std::nothrow) char[buff_len + 1]);
  if (buffer.get() == nullptr) {
    std::cerr << "Not enough memory to allocate " << buff_len + 1 << " bytes\n";
    exit(-1);
  }

  std::size_t window_len = 0;
  std::size_t hay_len = 0;

  std::size_t window_pos_in_file = 0;

  std::size_t padding = 0;
  std::size_t hay_start_pos = 0;

  while (in || hay_len < window_len) {
    if (in) {
      // read new text
      window_pos_in_file += window_len - padding;
      in.read(buffer.get() + padding,
              static_cast<std::streamsize>(buff_len - padding));
      const std::size_t read_len = static_cast<std::size_t>(in.gcount());
      window_len = padding + read_len;

      // generally, we will not search through the last chars
      // - the next iteration will search in these
      hay_len = window_len - AFIX_LEN;
      // but, if these are so few, they are the last in file => search in
      // these
      // also
      if (window_len <= AFIX_LEN) {
        hay_len = window_len; // this will also make this while's last iteration
      }
    } else {
      // reached the end of the file => just need to search in these last
      // chars
      // => search in these also
      hay_len = window_len; // this will also make this while's last iteration
    }

    // search through the haystack
    while (hay_start_pos + m_needle.size() < hay_len) {
      const char *const maybe_str =
          m_boyer_moore(buffer.get() + hay_start_pos, buffer.get() + hay_len)
              .first;
      if (maybe_str == buffer.get() + hay_len) {
        break;
      }

      const std::size_t str_position =
          static_cast<std::size_t>(maybe_str - buffer.get());

      // print new find
      out << file_name << '(' << window_pos_in_file + str_position << "):";

      // print prefix
      for (std::size_t i = AFIX_LEN; i > 0; --i) {
        if (maybe_str - i >= buffer.get()) {
          print_char(*(maybe_str - i), out);
        }
      }

      out << "...";

      // print suffix
      for (std::size_t i = 0; i < AFIX_LEN; ++i) {
        if (maybe_str + m_needle.size() + i < buffer.get() + window_len) {
          print_char(*(maybe_str + m_needle.size() + i), out);
        }
      }

      out << std::endl;

      hay_start_pos = str_position + 1;
    }

    // prepare the next window (with a new haystack)
    padding = AFIX_LEN + m_needle.size() + 2;
    if (in) {
      hay_start_pos = AFIX_LEN;
      // copy the last of this window to the begining of the next
      std::copy(buffer.get() + window_len - padding, buffer.get() + window_len,
                buffer.get());
    }
  }
}
