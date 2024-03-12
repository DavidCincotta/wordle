#include "wordleutils.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::vector<std::string> words;
unsigned int **bits;
int wordsize;

void run_thread(int start, int end, bool print) {
  // compare each word with every other word, and calculate the resulting
  // pattern
  for (int i = start; i < end; i++) {
    for (int j = 0; j < words.size(); j++) {
      std::string target = words[i];
      std::string word = words[j];
      std::string letters_left = word;
      for (int k = 0; k < target.size(); k++) {
        /*
        in wordle, the same letter in the same position is a green
        letter if the letter is used in the word, but not in the same
        position, it is a yellow letter it is only yellow if it is
        used and is still available from the correct word eg: "apple"
        "green" -> "iiiiy" The case I want to avoid is: "apple"
        "piles" -> "iyiyy where the "p" is used twice, but only one
        bit of the result is yellow since there is only one "p" in
        the word2
        */
        if (target[k] == word[k]) {
          bits[i][j] |= 3 << 2 * k;
          letters_left[k] = '-';
        } else {
          // not correct, preliminary storage of the incorrect signifier
          bits[i][j] |= 0 << 2 * k;
        }
      }
      for (int k = 0; k < target.size(); k++) {
        // if the letter is used, but not in the same position, it is a
        // yellow letter
        for (int l = 0; l < letters_left.size(); l++) {
          if (target[k] == letters_left[l]) {
            bits[i][j] |= 2 << 2 * k;
            letters_left[l] = '-';
            break;
          }
        }
      }
      // use groups bits to group the bits into groups of 2 and check if
      // there is a 3 and 2 in the same group if so print green and yellow
      if (print) {
        bool do_print = false;
        std::vector<int> groups = get_bit_groups(bits[i][j], 16, 2);
        for (int k = 0; k < groups.size(); k++) {
          for (int l = 0; l < groups.size(); l++) {
            if (groups[k] == 3 && groups[l] == 2) {
              do_print = true;
              break;
            }
          }
        }
        if (print && do_print) {
          std::cout << target << " " << word << " " << std::setw(5)
                    << bits[i][j] << " ";
          print_bits(std::cout, bits[i][j], 2 * wordsize, 2);
          std::cout << std::endl;
        }
      }
    }
    if (print && i % 1000 == 0) {
      std::cout << i << "\t" << words[i] << std::endl;
    }
  }
}
int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << "Usage: " << argv[0]
              << "./a.out <wordlist> <outputfile> <print> [thread]"
              << std::endl;
    return 1;
  }
  std::ifstream file(argv[1]);
  while (file) {
    std::string word;
    file >> word;
    words.push_back(word);
  }
  file.close();

  words.pop_back(); // empty line at the end
  wordsize = words[0].size();
  std::cout << "Number of words: " << words.size() << std::endl;
  std::cout << "Number of threads: " << std::thread::hardware_concurrency()
            << std::endl;

  std::cout << "Creating bit matrix" << std::endl;
  bits = new unsigned int *[words.size()];
  for (int i = 0; i < words.size(); i++) {
    bits[i] = new unsigned int[words.size()];
    for (int j = 0; j < words.size(); j++) {
      bits[i][j] = (unsigned int)0;
    }
  }

  if (argc == 5 && strcmp(argv[4], "thread") == 0) {
    std::cout << "Running threads" << std::endl;
    std::vector<std::thread> threads;
    int num_threads = std::thread::hardware_concurrency() - 1;
    int step = words.size() / num_threads;
    for (int i = 0; i < num_threads; i++) {
      int start = i * step;
      int end = i == num_threads - 1 ? words.size() + 1 : (i + 1) * step;
      std::cout << "Thread " << i + 1 << " " << start << " " << end
                << std::endl;
      threads.push_back(std::thread(run_thread, start, end, false));
    }
    for (int i = 0; i < num_threads; i++) {
      threads[i].join();
    }
    std::cout << "Threads finished" << std::endl;
  } else {
    std::cout << "Running single thread" << std::endl;
    run_thread(0, words.size(), atoi(argv[3]) == 1);
  }

CREATECSV:
  std::cout << "Creating csv" << std::endl;
  create_csv(bits, words, std::string(argv[2]));
  return 0;
}
