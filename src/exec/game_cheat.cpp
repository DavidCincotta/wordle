#include "PyString.h"
#include "wordleutils.h"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
/* TODO
 * this file is supposed to solve wordle, given that we input
 * a given pattern.
 *
 * This requires a rework of the WordleGame class to allow for
 * the input of a pattern, instead of actually picking a word
 *
 * It brings up the discussion of decoupling the solver
 * from the game itself.
 *
 * */
int main(int argc, char **argv) {
  WordleGame g = WordleGame(argv[1]);
  srand(time(0));
  WordleTurn t, next_turn;
  t = g;
  // while user input not end of file

  while (std::cin) {
    t.correct_word_index = rand() % g.words.size();
    t.possible_words_index = g.possible_words_index;
    t.is_correct = false;
    t.turn = 0;
    t.word = "";
    t.guesses.clear();
    t.word_index = 0;

    std::cout << std::endl
              << YELLOW << "\tNEW GAME\t" << RESET << std::endl
              << std::endl;

    while (!t.is_correct && t.possible_words_index.size() > 0) {
      std::cout << YELLOW << "Your best guess is: " << RED
                << g.words[t.calcualte_best_word()] << YELLOW
                << " with an expected info gain of: " << RED
                << std::setprecision(3)
                << t.calculate_entropy_of_guess(t.calcualte_best_word())
                << " bits" << RESET << std::endl;

      // get user input
      std::string guess;
      std::cout << MAGENTA << "Enter a guess: " << RESET;
      std::cin >> guess;
      unsigned guess_index = 0;
      for (unsigned i = 0; i < g.words.size(); i++) {
        if (g.words[i] == guess) {
          guess_index = i;
          break;
        }
      }
      std::cout << std::setw(15) << "" << t.word_to_pattern(guess_index)
                << std::endl << std::endl;

      unsigned prev_possible_words_index = t.possible_words_index.size();
      t.make_guess_in_place(guess_index);

      std::cout << YELLOW << "Actual info gain: " << RED
                << (log2(((double)prev_possible_words_index) /
                         t.possible_words_index.size()))
                << RESET << std::endl
                << std::endl;

      std::cout << YELLOW << "Here are the possible words: " << RESET;
      for (auto i : t.possible_words_index) {
        std::cout << g.words[i] << " ";
      }
      std::cout << std::endl << std::endl;
    }
    assert(t.is_correct);
    std::cout << CYAN << "You guessed the word " << t.word << " in "
              << ((t.turn > 6) ? RED : GREEN) << t.turn << CYAN << " turns"
              << RESET << std::endl;
  }
  return 0;
}
