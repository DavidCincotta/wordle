// PyString.cpp
#include "PyString.h"
#include <algorithm>
#include <sstream>

std::vector<std::string> PyString::split(const std::string &str,
                                         const std::string &delimiter) {
  std::vector<std::string> tokens;
  if (delimiter.length() == 0) {
    for (char c : str) {
      tokens.push_back(std::string(1, c));
    }
  } else {
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter[0])) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

std::string PyString::join(const std::vector<std::string> &parts,
                           const std::string &delimiter) {
  std::ostringstream joined;
  for (auto it = parts.begin(); it != parts.end(); ++it) {
    if (it != parts.begin()) {
      joined << delimiter;
    }
    joined << *it;
  }
  return joined.str();
}

std::string PyString::strip(const std::string &str, const std::string &chars) {
  size_t start = str.find_first_not_of(chars);
  size_t end = str.find_last_not_of(chars);
  return (start == std::string::npos || end == std::string::npos)
             ? ""
             : str.substr(start, end - start + 1);
}

std::string PyString::replace(const std::string &str, const std::string &from,
                              const std::string &to) {
  std::string result = str;
  size_t start_pos = 0;
  while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
    result.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return result;
}

int PyString::find(const std::string &str, const std::string &substr,
                   size_t start) {
  return str.find(substr, start);
}

std::string PyString::upper(const std::string &str) {
  std::string upperStr = str;
  std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
  return upperStr;
}

std::string PyString::lower(const std::string &str) {
  std::string lowerStr = str;
  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
  return lowerStr;
}

bool PyString::startswith(const std::string &str, const std::string &start) {
  return str.substr(0, start.length()) == start;
}

bool PyString::endswith(const std::string &str, const std::string &end) {
  if (end.length() > str.length())
    return false;
  return str.substr(str.length() - end.length()) == end;
}

size_t PyString::len(const std::string &str) { return str.length(); }
