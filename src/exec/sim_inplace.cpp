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
  t = g;
  mutex_count.lock();
  while (game_count < TOTAL_GAMES) {
    mutex_count.unlock();
    t.correct_word_index = rand() % g.words.size();
    t.possible_words_index = g.possible_words_index;
    t.is_correct = false;
    t.turn = 0;
    t.word = "";
    t.guesses.clear();
    t.word_index = 0;

    // Debug Printing

    while (!t.is_correct && t.possible_words_index.size() > 0) {

      t.make_best_guess_in_place();

      // Debug Printing
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
