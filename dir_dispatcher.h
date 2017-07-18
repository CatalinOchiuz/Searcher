/**
 *
 * dir_dispatcher.h
 *
 */

#ifndef SEARCH_FOR_STRING_DIR_DISPATCHER_H
#define SEARCH_FOR_STRING_DIR_DISPATCHER_H

#include "constants.h"
#include "file_searcher.h"
#include "utils.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <future>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = boost::filesystem;

/**
 * @brief The DirDispatcher class dispatches the search in a directory to all
 * files within, recursively, without following links. Follows the CRTP design
 * pattern - ImplT is this implementation for this "interface".
 */
template <class ImplT> class DirDispatcherCRTP {
public:
  /**
 * @brief DirDispatcher Constructor.
 * @param needle String to search for.
 */
  DirDispatcherCRTP(const std::string &needle) : m_file_searcher(needle) {}

  /**
   * @brief operator() dispatches the search in a directory to all files within,
   * recursively, without following links.
   * @param path Directory path to search into.
   */
  void operator()(const fs::path &path) {
    for (const auto &p : fs::recursive_directory_iterator(path)) {
      if (file_can_be_searched(p.path())) {
        static_cast<ImplT *>(this)->dispatch_search(p.path());
      }
    }
    static_cast<ImplT *>(this)->conclude_search();
  }

protected:
  /// FileSearcher encapsulating the actual search algorithm.
  const FileSearcher m_file_searcher;
};

/**
 * @brief The DirDispatcherSync class implements \p dispatch_search and \p
 * conclude_search methods for DirDispatcher following the CRTP design pattern.
 *
 * In this implementation, the file search is single-threaded.
 *
 */
class DirDispatcherSync : public DirDispatcherCRTP<DirDispatcherSync> {
public:
  /**
 * @brief DirDispatcherSync Constructor.
 * @param needle String to search for.
 */
  DirDispatcherSync(const std::string &needle) : DirDispatcherCRTP(needle) {}

  /**
   * @brief DirDispatcherSync Constructor; calls operator().
   * @param needle String to search for.
   * @param path Directory path to search into.
   */
  DirDispatcherSync(const std::string &needle, const fs::path &path)
      : DirDispatcherSync(needle) {
    operator()(path);
  }

  // friend needed to implement the second constructor.
  friend class DirDispatcherCRTP<DirDispatcherSync>;

private:
  /**
   * @brief dispatch_search implements a synchronous file search (by directly
   * using a FileSearcher).
   * @param path Directory to search into.
   */
  void dispatch_search(const fs::path &path) {
    m_file_searcher(path, std::cout);
  }

  /**
   * @brief conclude_search has nothing to do in this implementation.
   */
  void conclude_search() {}
};

/**
 * @brief The DirDispatcherAsync class implements \p dispatch_search and \p
 * conclude_search methods for DirDispatcher following the CRTP design pattern.
 *
 * In this implementation, the file searches may be multi-threaded.
 */
class DirDispatcherAsync : public DirDispatcherCRTP<DirDispatcherAsync> {
public:
  /**
 * @brief DirDispatcherAsync Constructor.
 * @param needle String to search for.
 */
  DirDispatcherAsync(const std::string &needle) : DirDispatcherCRTP(needle) {
    m_threads_num = std::thread::hardware_concurrency() / 2;
    if (m_threads_num == 0) {
      m_threads_num = DEFAULT_THREADS_NUM;
    }
    try {
      m_futures.reserve(m_threads_num);
    } catch (...) {
      std::cerr << "Not enough memory while allocating "
                << m_threads_num * sizeof(std::future<void>) << " bytes\n";
      exit(-1);
    }
  }
  /**
   * @brief DirDispatcherAsync Constructor; calls operator().
   * @param needle String to search for.
   * @param path Directory to search into.
   */
  DirDispatcherAsync(const std::string &needle, const fs::path &path)
      : DirDispatcherAsync(needle) {
    operator()(path);
  }

  // friend needed to implement the second constructor.
  friend class DirDispatcherCRTP<DirDispatcherAsync>;

private:
  /**
   * @brief dispatch_search implements an asynchronous file search by
   * dispatching work to a FileSearcher, maybe on a separate thread.
   * @param path Directory to search into.
   */
  void dispatch_search(const fs::path &path) {
    std::string file_name = path.string();
    std::size_t file_size = 0;
    try {
      file_size = fs::file_size(path);
    } catch (fs::filesystem_error &err) {
      std::cerr << "Error while getting the size of " << file_name << " : "
                << err.what() << std::endl;
      exit(-1);
    }

    // try not to launch threads for small files
    if (file_size > MIN_BUF_LEN_FOR_ASYNC) {
      std::future<void> handle;
      // one should not write on the same std::cout from multiple threads
      std::unique_ptr<std::ostringstream> out =
          std::make_unique<std::ostringstream>();
      try {
        handle = std::async(std::launch::deferred, m_file_searcher, path,
                            std::ref(*out));
      } catch (std::exception &e) {
        std::cerr << "Error while async start : " << e.what() << std::endl;
        exit(-1);
      }
      m_futures.push_back(std::move(handle));
      m_ostreams.push_back(std::move(out));
      if (m_futures.size() >= m_threads_num) {
        conclude_search();
      }
    } else {
      m_file_searcher(path, std::cout);
    }
  }
  /**
   * @brief conclude_search ensures that the asynchronous file searches are
   * waited for and the results are properly cout'ed.
   */
  void conclude_search() {
    // This whole strategy to do multithreading is quite simplistic...
    for (std::size_t i = 0; i < m_futures.size(); ++i) {
      m_futures[i].wait();
      std::cout << m_ostreams[i]->str();
    }
    m_futures.clear();
    m_ostreams.clear();
  }

  /// Maximum number of threads.
  unsigned int m_threads_num;
  /// Container of handles to all the std::async doing the file search.
  std::vector<std::future<void>> m_futures;
  /// Container of ostreams for all the results from async file searches.
  std::vector<std::unique_ptr<std::ostringstream>> m_ostreams;
};

#endif // SEARCH_FOR_STRING_DIR_DISPATCHER_H
