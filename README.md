# Wordle Solver

This project is an exploration of information theory, simulation, and multithreaded computing in C++.

The makefile should handle all the compilation necessary, but here are the steps to run this from scratch with a new set of words.

1) Insert a new list of words separated by newlines in the data directory as a txt file
2) Change the dataset field in the makefile
3) run createstats with the txt file as an argument, read the makefile for how to run it manually, or just call it using make
```bash
make createstats
```

This step caches the possible patterns between all words. This is an O(N^2) process, but greatly speeds up the computation. One thing to note, with large datasets, its always going to be helpful to pre-compute and hardcode the best first word. In my wordle.txt dataset, it was "tares" with an average of 6.3 bits of information.


### Motivation

After a conversation with a friend about information theory, I remembered a 3b1b video about the topic. It was so informative that no other resources were needed to complete the project. 

If you want to understand the theory of this project, go and watch [this video](https://www.youtube.com/watch?v=v68zYyaEmEA).

I did not implement a variable likelyhood of different words being correct. But the original algorithm actually had the same results as 3b1b, only my expected number of turns was slightly large because I had 2000 more words in my dataset. Is cool to notice that even with 20% more words, the expected number of turns increased less than 1%
