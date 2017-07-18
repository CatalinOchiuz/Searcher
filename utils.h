/**
  *
  * utils.h
  *
  */

#ifndef SEARCH_FOR_STRING_UTILS_H
#define SEARCH_FOR_STRING_UTILS_H

#include <boost/filesystem.hpp>
#include <iostream>

/**
 * @brief print_usage prints the expected usage of this program from command
 * line.
 * @param application Name of this program.
 */
void print_usage(const char *const application);

/**
 * @brief trimQuotationMarks trims quotation marks from the begining and the
 * end of the string, if any.
 * @param s String to trim.
 */
void trim_quotation_marks(std::string &s);

/**
 * @brief return_from_fs_call_or_exit returns the value received from \p fun or
 * exit(-1) if an exception is thrown.
 * @param fun Function to call.
 * @param param The parameter to be given to \p fun.
 * @return Return value from \p fun.
 */
bool return_from_fs_call_or_exit(bool (*fun)(const boost::filesystem::path &),
                                 const boost::filesystem::path &param);

/**
 * @brief file_can_be_searched tests if a file is searcheable (regular file but
 * not symlink)
 * @param file_path File path to test.
 * @return Return true if this file is searchable and false otherwise.
 */
bool file_can_be_searched(const boost::filesystem::path &file_path);

/**
 * @brief print_char prints newline as "\\n", tab as "\\t", and all other
 * characters verbatim.
 * @param c Char to print.
 * @param out ostream to write to.
 */
void print_char(const char c, std::ostream &out);

#endif // SEARCH_FOR_STRING_UTILS_H
