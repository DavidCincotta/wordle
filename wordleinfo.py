# this program solves wordle puzzles using information theory
# and the wordle word list to find the best word to guess next
# based on the information gained from previous guesses
#
# entropy = -sum(p(x) * log2(p(x)))
# information gain = entropy before - entropy after
# we want to maximize information gain for a given guess
import math
import random
from typing import List


# read the wordle word list
def read_word_list():
    word_list = []
    with open('data/wordle.txt', 'r') as f:
        for line in f:
            word_list.append(line.strip())
    return word_list


def entropy(a:List[float]):
    return -sum([p * math.log2(p) for p in a])

word_list = set(read_word_list())

# print("The entropy of the word list is:",round(entropy([1.0/len(word_list) for _ in range(len(word_list))]),2))


hidden_word = random.choice(list(word_list)) # the wordle word
correct_letters = ['_' for _ in range(5)] # the wordle word, also contains the position of the correct letters
guessed_letters = set() # the letters guessed so far
guesses = [] # the words guessed so far
used_letters = set() # the letters in the wordle word
incorrect_letters = set() # the letters that are not in the wordle word
possible_words = word_list.copy() # the words that are still possible

def reset():
    global hidden_word,correct_letters,guessed_letters,guesses,used_letters,incorrect_letters,possible_words
    correct_letters = ['_' for _ in range(5)]
    guessed_letters = set()
    guesses = []
    used_letters = set()
    incorrect_letters = set()
    possible_words = word_list.copy()
    hidden_word = random.choice(list(word_list))


#calculate the probability of a word being the wordle word given the information we have
def probability(word:str)->float:
    if word in possible_words:
        return 1.0/len(possible_words)
    return 0.0

def prune_words(wordset:set):
    to_remove = set()
    for word in possible_words:
        for i in range(5):
            if word[i] in incorrect_letters:
                to_remove.add(word)
                break
            if correct_letters[i] != '_' and word[i] != correct_letters[i]:
                to_remove.add(word)
                break
    wordset.difference_update(to_remove)

def get_entropy_of_possible():
    return entropy([probability(word) for word in possible_words])


def random_guess():
    return random.choice(list(possible_words))
def random_not_possible_guess():
    if len(word_list.difference(possible_words)) == 0:
        return random_guess()
    return random.choice(list(word_list.difference(possible_words)))
def random_guess_no_incorrect():
    possible_guesses = word_list.copy()
    for word in word_list:
        for letter in word:
            if letter in incorrect_letters:
                possible_guesses.remove(word)
                break
            if letter in used_letters:
                possible_guesses.remove(word)
                break
    if len(possible_guesses) == 0:
        possible_guesses = word_list.copy()
        for word in word_list:
            for letter in word:
                if letter in incorrect_letters:
                    possible_guesses.remove(word)
                    break
    #sort list by number of unique letters
    possible_guesses = sorted(possible_guesses,key=lambda x:len(set(x)))
    return random.choice(list(possible_guesses))

def get_entropy_of_possible_given_guess(guess:str):
    pass


def guess_most_information_gain():
    max_gain = 0
    best_guess = ''
    for word in word_list:
        gain = get_entropy_of_possible() - get_entropy_of_possible_given_guess(word)
        if gain > max_gain:
            max_gain = gain
            best_guess = word
    return best_guess

def update(word:str):
    guesses.append(word)
    for i in range(5):
        if word[i] == hidden_word[i]:
            correct_letters[i] = word[i]
    guessed_letters.update(set(word))
    for letter in word:
        if letter not in hidden_word:
            incorrect_letters.add(letter)
        if letter in hidden_word:
            used_letters.add(letter)
    prune_words(possible_words)


def print_info():
    print("Correct letters:".ljust(30),correct_letters)
    print("Used letters:".ljust(30),used_letters)
    print("Guessed letters:".ljust(30),guessed_letters)
    print("Incorrect letters:".ljust(30),incorrect_letters)
    print("Possible words:".ljust(30),len(possible_words))
    print("Entropy of possible words:".ljust(30),round(get_entropy_of_possible(),2))



def standard_run():
    # print("The wordle word is:",hidden_word)
    while hidden_word not in guesses:
        # guess = random_guess_no_incorrect()
        guess = random_guess()
        # guess = random_not_possible_guess()
        # guess = guess_most_information_gain()
        # print("\tGuessing:",guess)
        update(guess)
        # print_info()
        # print("\n")
    t = len(guesses)
    reset()
    return t

def run_trials(n:int):
    results = [standard_run() for _ in range(n)]
    # print("Results:",results)
    print("Avg:",sum(results)/float(n))

# run_trials(1000)

run_trials(10000)

