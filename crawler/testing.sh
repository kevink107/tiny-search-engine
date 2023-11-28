#!/bin/bash
# Kevin King
# Crawler Test
# CS50 Winter 2022
# Feburary 6, 2022

# Clear directory:
rm -rf ../data/crawler/*
mkdir ../data
mkdir ../data/crawler

# No arguments 
./crawler 

# INVALID (1 ARGUMENT)
./crawler x

# INVALID (2 ARGUMENTS)
./crawler x y

# INVALID (3 ARGUMENTS)
./crawler x y z

# INVALID DIRECTORY
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html directory 5

# DEPTH TOO LOW
mkdir ../data/crawler/invalid-range
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/invalid-range -1

# DEPTH TOO HIGH
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/invalid-range 11

# DEPTH 0 (letters)
mkdir ../data/crawler/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/letters-0 0

# DEPTH 1 (letters)
mkdir ../data/crawler/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/letters-1 1

# DEPTH 2 (letters)
mkdir ../data/crawler/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/letters-2 2

# DEPTH 10 (letters) 
mkdir ../data/crawler/letters-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/crawler/letters-10 10

# DEPTH 2 (letters/A)
mkdir ../data/crawler/lettersA-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../data/crawler/lettersA-2 2

# DEPTH 0 (toscrape)
mkdir ../data/crawler/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/crawler/toscrape-0 0

# DEPTH 0 (wikipedia)
mkdir ../data/crawler/wiki-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/crawler/wiki-0 0

# Files created:
ls ../data/

exit 0
