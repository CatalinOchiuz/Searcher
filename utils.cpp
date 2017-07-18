/**
  *
  * utils.cpp
  *
  */

#include "utils.h"
#include <iostream>

namespace fs = boost::filesystem;

void print_usage(const char *const application) {
  std::string app_name;

  try {
    app_name = fs::path(application).filename().string();
  } catch (...) {
    app_name = application;
  }

  std::cerr << "Usage:\n"
            << app_name << " FILE STRING\n"
            << app_name << " DIRECTORY STRING" << std::endl;
}

void trim_quotation_marks(std::string &s) {
  if (s.size() > 1 && s[s.size() - 1] == '"') {
    s.erase(s.size() - 1);
  }
  if (s[0] == '"') {
    s.erase(0, 1);
  }
}

bool return_from_fs_call_or_exit(bool (*fun)(const fs::path &),
                                 const fs::path &param) {
  try {
    return fun(param);
  } catch (std::exception &err) {
    std::cerr << "Error while inspecting : " << param.string() << " : "
              << err.what() << std::endl;
    exit(-1);
  }
}

bool file_can_be_searched(const fs::path &file_path) {
  const bool is_regular_file =
      return_from_fs_call_or_exit(fs::is_regular_file, file_path);
  const bool is_symlink = return_from_fs_call_or_exit(fs::is_symlink, file_path);

  return is_regular_file && !is_symlink;
}

void print_char(const char c, std::ostream &out) {
  if (c != '\n' && c != '\t') {
    out << c;
    return;
  }
  if (c == '\n') {
    out << "\\n";
    return;
  }
  out << "\\t";
}
