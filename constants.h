/**
  *
  * constants.h
  *
  */

#ifndef SEARCH_FOR_STRING_CONSTANTS_H
#define SEARCH_FOR_STRING_CONSTANTS_H

#include <cstddef>

/// length of the prefix and suffix
const std::size_t AFIX_LEN = 3;
/// max length of the needle
const std::size_t MAX_NEEDLE_LEN = 128;
/// min length of buffer read at once
const std::size_t BUF_LEN_MIN = AFIX_LEN + MAX_NEEDLE_LEN + 2;
/// max length of buffer read at once
const std::size_t BUF_LEN_MAX = 500 * 1024;
/// min length to consider spawning a parallel search
const std::size_t MIN_BUF_LEN_FOR_ASYNC = BUF_LEN_MAX * 100;
const std::size_t DEFAULT_THREADS_NUM = 2;

static_assert(BUF_LEN_MIN <= BUF_LEN_MAX, "min > max");
static_assert(2 * AFIX_LEN < BUF_LEN_MIN,
              "The algorithm depends on this inequality");

#endif // SEARCH_FOR_STRING_CONSTANTS_H
