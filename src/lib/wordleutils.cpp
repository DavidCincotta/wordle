#include "wordleutils.h"
#include "PyString.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> WordleGame::words;
std::string WordleGame::filename;
unsigned int **WordleGame::bits;
int WordleGame::wordsize;

std::vector<int> get_bit_groups(unsigned int n, int bits, int group) {
  std::vector<int> groups;
  // group bits into groups of group, eg bits = 8, group = 2 -> 00 00 00 00
  for (int i = 0; i < bits; i += group) {
    int group_bits = 0;
    for (int j = 0; j < group; j++) {
      group_bits |= (n & 1) << j;
      n >>= 1;
    }
    groups.push_back(group_bits);
  }
  return groups;
}
// print the bits of a number
void print_bits(std::ostream &ss, unsigned int n, int bits, int group) {
  for (int i = 0; i < bits; i++) {
    ss << (n & 1);
    if ((i + 1) % group == 0) {
      ss << " ";
    }
    n >>= 1;
  }
}

void create_csv(unsigned int **bits, std::vector<std::string> words,
                std::string filename) {
  std::ofstream bitfile(filename);
  // header
  bitfile << words[0].size() << ",";
  for (size_t i = 0; i < words.size(); i++) {
    bitfile << words[i];
    if (i != words.size() - 1)
      bitfile << ",";
  }
  bitfile << std::endl;
  for (size_t i = 0; i < words.size(); i++) {
    bitfile << words[i] << ",";
    for (size_t j = 0; j < words.size(); j++) {
      bitfile << bits[i][j];
      if (j != words.size() - 1)
        bitfile << ",";
    }
    bitfile << std::endl;
  }
}

std::string WordleGame::word_to_pattern(unsigned word_index) {
  std::vector<std::string> split = PyString::split(words[word_index], "");
  std::vector<int> groups = get_bit_groups(bits[word_index][correct_word_index],
                                           WordleGame::wordsize * 2, 2);
  for (size_t i = 0; i < split.size(); i++) {
    if (groups[i] == 0) {
      split[i] = RED + split[i] + RESET;
    } else if (groups[i] == 2) {
      split[i] = BLUE + split[i] + RESET;
    } else {
      split[i] = GREEN + split[i] + RESET;
    }
  }
  return PyString::join(split, "");
}
void WordleGame::load_bit_matrix() {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: file {" << filename << "} not found" << std::endl;
    return;
  }
  std::string header;
  getline(file, header);
  words = PyString::split(header, ",");
  wordsize = (unsigned)std::stoi(words[0]);
  words.erase(words.begin()); // remove the first element, top left corner which
                              // is the word size
  // std::cout << "words: " << words.size() << std::endl
  //           << "wordsize: " << wordsize << std::endl;
  // std::cout << "Allocating Memory" << std::endl;
  bits = new unsigned int *[words.size()];
  for (size_t i = 0; i < words.size(); i++) {
    bits[i] = new unsigned int[words.size()];
  }
  // std::cout << "Loading Bitmatrix" << std::endl;
  // std::chrono::high_resolution_clock::time_point start, end;
  // start = std::chrono::high_resolution_clock::now();
  // double elapsed_seconds_split = 0;
  // double elapsed_seconds_stoi = 0;
  for (size_t i = 0; i < words.size(); i++) {
    std::string line;
    // calc runtime
    getline(file, line);
    // std::chrono::high_resolution_clock::time_point s, e;
    // s = std::chrono::high_resolution_clock::now();
    std::vector<std::string> line_split = PyString::split(line, ",");
    // e = std::chrono::high_resolution_clock::now();
    // elapsed_seconds_split +=
    //     std::chrono::duration_cast<std::chrono::duration<double>>(e - s)
    //         .count();
    // s = std::chrono::high_resolution_clock::now();
    for (size_t j = 1; j <= WordleGame::words.size(); j++) {
      bits[i][j - 1] = (unsigned)std::stoi(line_split[j]);
    }
    // e = std::chrono::high_resolution_clock::now();
    // elapsed_seconds_stoi +=
    //     std::chrono::duration_cast<std::chrono::duration<double>>(e - s)
    //         .count();
  }
  file.close();
  // std::cout << "Bitmatrix Loaded" << std::endl;
  // end = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double> elapsed_seconds = end - start;
  // std::cout << "Elapsed time: " << elapsed_seconds.count() << "s" <<
  // std::endl; std::cout << "Time Split:   " << elapsed_seconds_split << "s" <<
  // std::endl; std::cout << "Time stoi:    " << elapsed_seconds_stoi << "s" <<
  // std::endl; std::cout << "Observe Cost: "
  //           << elapsed_seconds.count() -
  //                  (elapsed_seconds_split + elapsed_seconds_stoi)
  //           << "s" << std::endl;
  // std::cout << "Header Check" << std::endl;
  // for (size_t i = 0; i < 2; i++) {
  //   std::cout << words[i] << " ";
  //   for (size_t j = 0; j < 10; j++) {
  //     std::cout << bits[i][j] << " ";
  //   }
  //   std::cout << std::endl;
  // }
}

WordleGame::WordleGame(std::string filename) {
  WordleGame::filename = filename;
  load_bit_matrix();
  for (size_t i = 0; i < words.size(); i++) {
    possible_words_index.insert(i);
  }
  correct_word_index = rand() % words.size();
}
WordleGame::WordleGame() {}
WordleGame::~WordleGame() {}

WordleTurn::WordleTurn() {
  turn = 0;
  is_correct = false;
}
WordleTurn::~WordleTurn() {}

WordleTurn &WordleTurn::operator=(WordleGame &&rhs) {
  possible_words_index = rhs.possible_words_index;
  turn = 0;
  correct_word_index = rhs.correct_word_index;
  is_correct = false;
  return *this;
}
WordleTurn &WordleTurn::operator=(const WordleGame &rhs) {
  possible_words_index = rhs.possible_words_index;
  turn = 0;
  correct_word_index = rhs.correct_word_index;
  is_correct = false;
  return *this;
}
WordleTurn WordleTurn::make_guess(unsigned guess_index) {
  WordleTurn t = *this;
  t.word = words[guess_index];
  t.word_index = guess_index;
  if (guess_index == correct_word_index) {
    t.is_correct = true;
  }
  t.turn++;
  t.guesses.push_back(guess_index);
  // shrink the possible guesses
  std::unordered_set<unsigned> new_possible_words_index;
  for (auto i : possible_words_index) {
    // compare the pattern that we recieved from the guess with the pattern of
    // all possible guesses, the correct word will be in this set
    //
    // to play a real game, we need to input this pattern manually
    if (bits[guess_index][correct_word_index] == bits[guess_index][i]) {
      new_possible_words_index.insert(i);
    }
  }
  t.possible_words_index = new_possible_words_index;
  return t;
}
void WordleTurn::make_guess_in_place(unsigned guess_index) {
  word = words[guess_index];
  word_index = guess_index;
  if (guess_index == correct_word_index) {
    is_correct = true;
  }
  turn++;
  guesses.push_back(guess_index);
  // shrink the possible guesses
  std::unordered_set<unsigned> new_possible_words_index;
  for (auto i : possible_words_index) {
    // compare the pattern that we recieved from the guess with the pattern of
    // all possible guesses, the correct word will be in this set
    //
    // to play a real game, we need to input this pattern manually
    if (bits[guess_index][correct_word_index] == bits[guess_index][i]) {
      new_possible_words_index.insert(i);
    }
  }
  possible_words_index = new_possible_words_index;
}

double WordleTurn::calculate_entropy_of_guess(unsigned guess_word_index) {
  double entropy = 0;
  // create a map from unsigned to a vec of unsigned
  std::unordered_map<unsigned, std::vector<unsigned>> pattern_to_words;
  for (unsigned possible_correct_word : possible_words_index) {
    unsigned pattern = bits[guess_word_index][possible_correct_word];
    pattern_to_words[pattern].push_back(possible_correct_word);
  }
  unsigned count = 0;
  for (auto i : pattern_to_words) {
    double p = (double)i.second.size() / possible_words_index.size();
    entropy += -p * log2(p);
    count += i.second.size();
  }
  assert(count == possible_words_index.size());
  return entropy;
}
unsigned WordleTurn::calcualte_best_word() {
  unsigned best_word_index = 0;
  double best_entropy = 0;
  for (unsigned i = 0; i < words.size(); i++) {
    double entropy = calculate_entropy_of_guess(i);
    if (entropy > best_entropy) {
      best_entropy = entropy;
      best_word_index = i;
    }
  }
  // we prefer to use guesses that could be the correct word
  if (words.size() > possible_words_index.size()) {
    for (auto i : possible_words_index) {
      double entropy = calculate_entropy_of_guess(i);
      if (entropy >= best_entropy) {
        best_entropy = entropy;
        best_word_index = i;
      }
    }
  }
  return best_word_index;
}
WordleTurn WordleTurn::make_best_guess() {
  if (turn == 0 && filename == "data/wordle.csv")
    return make_guess(std::find(words.begin(), words.end(), "tares") -
                      words.begin());
  if (possible_words_index.size() == 1)
    return make_guess(*possible_words_index.begin());
  return make_guess(calcualte_best_word());
}
WordleTurn WordleTurn::make_random_guess() {
  unsigned temp = 0;
  while (possible_words_index.find(temp) == possible_words_index.end())
    temp++;
  return make_guess(temp);
}
void WordleTurn::make_best_guess_in_place() {
  if (turn == 0 && filename == "data/wordle.csv") {
    make_guess_in_place(std::find(words.begin(), words.end(), "tares") -
                        words.begin());
    return;
  }
  if (possible_words_index.size() == 1) {
    make_guess_in_place(*possible_words_index.begin());
    return;
  }
  make_guess_in_place(calcualte_best_word());
}
void WordleTurn::make_random_guess_in_place() {
  unsigned temp = 0;
  while (possible_words_index.find(temp) == possible_words_index.end())
    temp++;
  make_guess_in_place(temp);
}
