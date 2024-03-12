#include "PyString.h"
#include "wordleutils.h"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
WordleGame g;
std::mutex mutex_count = std::mutex();
std::mutex mutex_print = std::mutex();
int turn_count = 0;
int game_count = 0;
int turn_distribution[30] = {0};
#define TOTAL_GAMES 10000
#define THREADS 10

void play_games() {
  WordleTurn t, next_turn;
  mutex_count.lock();
  while (game_count < TOTAL_GAMES) {
    mutex_count.unlock();
    t = g;
    t.correct_word_index = rand() % g.words.size();

    // Debug Printing
    std::stringstream ss;

    ss << "The word is: " << t.words[t.correct_word_index] << std::endl;

    while (!t.is_correct && t.possible_words_index.size() > 0) {

      // Debug Printing

      std::chrono::high_resolution_clock::time_point start, end;
      start = std::chrono::high_resolution_clock::now();

      next_turn = t.make_best_guess();
      // next_turn = t.make_random_guess();

      // Debug Printing

      end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      ss << "Turn:" << std::setw(2) << next_turn.turn << " " << next_turn.word
         << (next_turn.is_correct ? GREEN : RED)
         << (next_turn.is_correct ? " right" : " wrong") << RESET << " ="
         << std::setw(7) << next_turn.word_index << " Pattern: " << std::setw(4)
         << next_turn.bits[next_turn.word_index][next_turn.correct_word_index]
         << " = ";
      print_bits(
          ss,
          next_turn.bits[next_turn.word_index][next_turn.correct_word_index],
          10, 2);
      ss << "Words:" << std::setw(7) << next_turn.possible_words_index.size()
         << " Time: " << std::setw(12) << elapsed_seconds.count()
         << " Entropy: " << std::setw(10)
         << t.calculate_entropy_of_guess(next_turn.word_index) << std::endl;

      ss << "Possible Words: ";
      for (auto i : next_turn.possible_words_index) {
        ss << g.words[i] << " = " << std::setw(3)
           << next_turn.calculate_entropy_of_guess(i) << ", ";
      }
      ss << std::endl;

      if (game_count < 10 && THREADS == 1)
        std::cout << ss.str();

      t = next_turn;
    }
    assert(t.is_correct);

    mutex_count.lock();
    turn_distribution[t.turn]++;
    turn_count += t.turn;
    if (game_count % 100 == 0) {
      std::cerr << CYAN << "Game: " << std::setw(5) << game_count
                << " Turns: " << std::setw(5) << turn_count
                << " Avg: " << std::setw(5) << (double)turn_count / game_count
                << RESET << std::endl;
    }
    game_count++;
    mutex_count.unlock();
  }
}

int main(int argc, char **argv) {
  srand(time(0));
  g = WordleGame(argv[1]);

  std::thread threads[THREADS] = {};
  for (int i = 0; i < THREADS; i++) {
    threads[i] = std::thread(play_games);
  }
  for (int i = 0; i < THREADS; i++) {
    threads[i].join();
  }

  std::cout << std::endl
            << MAGENTA << "Game: " << std::setw(5) << game_count
            << " Turn: " << std::setw(5) << turn_count
            << " Avg: " << std::setw(5) << (double)turn_count / game_count
            << RESET << std::endl;
  for (int i = 0; i < 15; i++) {
    std::cout << "Turn" << std::setw(3) << i << ": " << turn_distribution[i]
              << std::endl;
  }

  return 0;
}
