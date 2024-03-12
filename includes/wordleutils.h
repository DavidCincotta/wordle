#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<int> get_bit_groups(unsigned int n, int bits = 32, int group = 1);
void print_bits(std::ostream &ss, unsigned int n, int bits = 32,
                int group = 32);
void create_csv(unsigned int **bits, std::vector<std::string> words,
                std::string filename = "bitstats.csv");
// void load_bit_matrix(std::string filename, std::vector<std::string> &words,
//                      unsigned int **bits, int *wordsize);

class WordleGame {
public:
  static std::vector<std::string> words;
  static std::string filename;
  static unsigned int **bits;
  static int wordsize;
  unsigned correct_word_index;
  std::unordered_set<unsigned> possible_words_index;
  WordleGame();
  WordleGame(std::string filename);
  std::string word_to_pattern(unsigned word_index);
  WordleGame(const WordleGame &g) = default;
  void load_bit_matrix();
  ~WordleGame();
};
class WordleTurn : public WordleGame {
public:
  std::string word;
  unsigned word_index;
  int turn;
  bool is_correct;
  std::vector<unsigned> guesses;
  double calculate_entropy_of_guess(unsigned guess_word_index);
  unsigned calcualte_best_word();
  WordleTurn make_guess(unsigned guess_word_index);
  WordleTurn make_random_guess();
  WordleTurn make_best_guess();
  void make_guess_in_place(unsigned guess_word_index);
  void make_random_guess_in_place();
  void make_best_guess_in_place();

  WordleTurn();
  ~WordleTurn();

  WordleTurn &operator=(WordleGame &&);
  WordleTurn &operator=(const WordleGame &);

  // WordleTurn &operator=(const WordleTurn &) = default;
  // WordleTurn &operator=(WordleTurn &&) = default;
};
