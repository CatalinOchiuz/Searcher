/**
 * Search a string in a file or in a directory, recursively.
 *
 * When searching a file, use the Boyer-Moore algorithm.
 * But space is a concern when dealing with big files, so a file is read one
 * buffer at a time. To be able to find the string which happens to be starting
 * at the end of one buffer and ending in the next, a padding is used and
 * carried around from the end of one buffer to the beginning of the next.
 *
 * Some files may be searched asynchronously in order to speed up the process.
 *
 * TODO: character encoding
 *
 */

#include "constants.h"
#include "dir_dispatcher.h"
#include "file_searcher.h"
#include "utils.h"
#include <boost/filesystem.hpp>
#include <string>

namespace fs = boost::filesystem;

int main(const int argc, const char *const argv[]) {
  if (argc != 3) {
    std::cerr << "Wrong number of arguments\n";
    print_usage(argv[0]);
    return -1;
  }
  if (argv[1][0] == '\0' || argv[2][0] == '\0') {
    std::cerr << "Empty parameter\n";
    print_usage(argv[0]);
    return -1;
  }
  if (strlen(argv[2]) > MAX_NEEDLE_LEN) {
    std::cerr << "STRING longer than " << MAX_NEEDLE_LEN << '\n';
    print_usage(argv[0]);
    return -1;
  }

  fs::path path(argv[1]);
  std::string needle(argv[2]);

  bool exists = return_from_fs_call_or_exit(fs::exists, path);
  if (!exists) {
    std::string name = path.string();
    trim_quotation_marks(name);
    std::cerr << "File not found: " << name << std::endl;
    print_usage(argv[0]);
  } else if (file_can_be_searched(path)) {
    FileSearcher(needle, path, std::cout);
  } else {
    bool is_directory = return_from_fs_call_or_exit(fs::is_directory, path);
    if (is_directory) {
      ///
      /// How to choose between using the Async- and the Sync- flavors of
      /// DirDispatcherCRTP:
      ///
      /// DirDispatcherSync   Uses very little memory, but it is not very fast.
      /// DirDispatcherAsync  Very fast, but uses more memory.
      ///
      DirDispatcherAsync(needle, path);
    }
  }

  return 0;
}
