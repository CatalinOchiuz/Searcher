/**
  *
  * file_searcher.h
  *
  */

#ifndef SEARCH_FOR_STRING_FILE_SEARCHER_H
#define SEARCH_FOR_STRING_FILE_SEARCHER_H

#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/filesystem.hpp>
#include <string>

/**
 * @brief The FileSearcher class searches a string in a file;
 * operator() can be used to do the search.
 * The second constructor calls operator() itself, for convenience.
 *
 */
class FileSearcher {
public:
  /**
  *@brief FileSearcher Constructor.
  *@param needle String to search for. Its contents must not change during a
  *FileSearcher lifetime.
  */
  FileSearcher(const std::string &needle);

  /**
   * @brief FileSearcher forwards its parameters to operator(), which searches
   * \p needle in file \p path.
   * @param path Path to the file to search in.
   * @param needle String to search for. Its constents must not change during a
   * FileSearcher lifetime.
   * @param path Path to a file to search into.
   * @param out ostream to write search results to.
   */
  FileSearcher(const std::string &needle, const boost::filesystem::path &path,
               std::ostream &out);

  /**
   * @brief operator () searchs \p needle in file \p path using
   * \p boyer_moore; uses a buffer to avoid loading the file in memory at once.
   * @param path Path to the file to search into.
   * @param out ostream to write search results to.
   */
  void operator()(const boost::filesystem::path &path, std::ostream &out) const;

private:
  const std::string &m_needle;
  const boost::algorithm::boyer_moore<std::string::const_iterator>
      m_boyer_moore;
};

#endif // SEARCH_FOR_STRING_FILE_SEARCHER_H
