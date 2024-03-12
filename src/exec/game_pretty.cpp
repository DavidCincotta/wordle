#include "wordleutils.h"
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <string>
int main(int argc, char **argv) {
  WordleGame g = WordleGame(argv[1]);
  srand(time(nullptr));
  int help = 0;
  if (argc > 2)
    help = std::stoi(argv[2]);
  // Initialize ncurses
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // Define color pairs
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Instructions and messages
  init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Correct guesses
  init_pair(3, COLOR_RED, COLOR_BLACK);     // Incorrect guesses
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // User input
  init_pair(5, COLOR_CYAN, COLOR_BLACK);    // Word pattern
  init_pair(6, COLOR_WHITE, COLOR_BLACK);   // WordleTurn

  WordleTurn t;
  t = g;

  while (true) {
    t.correct_word_index = rand() % g.words.size();
    t.possible_words_index = g.possible_words_index;
    t.is_correct = false;
    t.turn = 0;
    t.word = "";
    t.guesses.clear();
    t.word_index = 0;

    int maxX = getmaxx(stdscr), maxY = getmaxy(stdscr);

    if (help)
      mvprintw(0, 0,
               "Max X: %d Max Y: %d\nCorrect Word: %s\nPossible Words: %zu",
               maxX, maxY, g.words[t.correct_word_index].c_str(),
               t.possible_words_index.size());

    int start_x = 4, start_y = (int)(maxX / 2) - (int)(g.wordsize / 2) - 1;
    int x = start_x, y = start_y;
    while (!t.is_correct && t.possible_words_index.size() > 0) {
      if (help) {
        // clearing the help message
        mvprintw(0, 0,
                 "Max X: %d Max Y: %d\nCorrect Word: %s\nPossible Words: %s",
                 maxX, maxY, g.words[t.correct_word_index].c_str(), "      ");
        // printing help message
        mvprintw(0, 0,
                 "Max X: %d Max Y: %d\nCorrect Word: %s\nPossible Words: %zu",
                 maxX, maxY, g.words[t.correct_word_index].c_str(),
                 t.possible_words_index.size());
        mvprintw(3, 0, "Samples:");
        auto it = t.possible_words_index.begin();
        for (int i = 0; i < 100; i++) {
          if (it == t.possible_words_index.end()) {
            mvprintw(4 + i, 0, "%s", "          ");
          } else {
            mvprintw(4 + i, 0, "%s", g.words[*it++].c_str());
          }
        }
      }
      char guess[100];
      attron(COLOR_PAIR(5)); // Instructions
      mvprintw(x, y, "");
      echo();
      getnstr(guess, 99);
      noecho();

      int guess_index = -1;
      for (int i = 0; i < g.words.size(); i++) {
        if (g.words[i] == std::basic_string(guess)) {
          guess_index = i;
          break;
        }
      }
      if (guess_index == -1) {
        attron(COLOR_PAIR(1));
        mvprintw(start_x - 2, start_y, "Invalid guess");
        attroff(COLOR_PAIR(1));
        continue;
      } else {
        attron(COLOR_PAIR(1));
        mvprintw(start_x - 2, start_y, "             ");
        attroff(COLOR_PAIR(1));
      }

      // Assuming t.word_to_pattern returns a string representing the guess
      // result
      std::vector<int> pattern = get_bit_groups(
          t.bits[guess_index][t.correct_word_index], t.wordsize * 2, 2);
      for (int i = 0; i < t.wordsize; i++) {
        switch (pattern[i]) {
        case 0:
          attron(COLOR_PAIR(6));
          break;
        case 2:
          attron(COLOR_PAIR(1));
          break;
        case 3:
          attron(COLOR_PAIR(2));
          break;
        }
        mvprintw(x, y++, "%c", guess[i]);
        switch (pattern[i]) {
        case 0:
          attroff(COLOR_PAIR(6));
          break;
        case 2:
          attroff(COLOR_PAIR(1));
          break;
        case 3:
          attroff(COLOR_PAIR(2));
          break;
        }
      }
      y = start_y;
      x++;

      t.make_guess_in_place(guess_index);

      if (t.is_correct) {
        attron(COLOR_PAIR(2)); // Correct guess
        mvprintw(start_x - 2, start_y, "Correct! The word was %s",
                 t.word.c_str());
        attroff(COLOR_PAIR(2));
      }
      t.turn++;
    }

    getch(); // Wait for user to press a key before starting a new game
    clear(); // Clear the screen for the new game
  }

  // Cleanup ncurses
  endwin();

  return 0;
}
