/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/21/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */
#include "../../include/utilities/UtilityFunctions.hh"
#include <random>

using namespace chase;

template <typename T>
void chase::getSubsetBySize(std::vector<T> elements, unsigned int size,
                            unsigned int left, unsigned int index,
                            std::vector<T> &combination,
                            std::list<std::vector<T>> &results) {
  if (left == 0) {
    results.push_back(combination);
    return;
  }
  for (unsigned int i = index; i < size; ++i) {
    combination.push_back(elements[i]);
    getSubsetBySize(elements, size, left - 1, i + 1, combination, results);
    combination.pop_back();
  }
}

template <>
void chase::getSubsetBySize(std::vector<std::string> elements,
                            unsigned int size, unsigned int left,
                            unsigned int index,
                            std::vector<std::string> &combination,
                            std::list<std::vector<std::string>> &results) {
  if (left == 0) {
    results.push_back(combination);
    return;
  }
  for (unsigned int i = index; i < size; ++i) {
    combination.push_back(elements[i]);
    getSubsetBySize(elements, size, left - 1, i + 1, combination, results);
    combination.pop_back();
  }
}

std::string chase::getRandomStr(int len, std::string prefix,
                                std::string suffix) {
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

  std::string ret(std::move(prefix));
  for (int i = 0; i < len; ++i)
    ret += alphanum[dis(gen)];
  ret += suffix;
  return ret;
}
