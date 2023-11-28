#!/bin/bash
# Kevin King
# Indexer Test
# CS50 Winter 2022
# Feburary 11, 2022

# Clear directory:
rm -rf ../data/indexer/*
rm -rf ../data/crawler/*
mkdir -p ../data
mkdir -p ../data/crawler
mkdir -p ../data/indexer

# MAKE CRAWLER DIRECTORIES AND RUN CRAWLER
mkdir ../data/crawler/letters-1
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/crawler/letters-1 1

mkdir ../data/crawler/letters-2
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/crawler/letters-2 2

mkdir ../data/crawler/letters-3
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/crawler/letters-3 3

mkdir ../data/crawler/letters-10
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/crawler/letters-10 10

mkdir ../data/crawler/toscrape-0
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/crawler/toscrape-0 0

mkdir ../data/crawler/wiki-0
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/crawler/wiki-0 0

# NO ARGUMENTS
./indexer 

# INVALID (1 ARGUMENT)
./indexer x

# INVALID (3 ARGUMENTS)
./indexer x y z

# INVALID DIRECTORY (NON-EXISTENT PATH)
./indexer nonExistentPath ../data/indexer/letters-1.index

# INVALID DIRECTORY (NOT CRAWLER DIRECTORY)
mkdir ../data/crawler/emptyCrawlerDirectory
./indexer ../data/crawler/emptyCrawlerDirectory ../data/indexer/letters-1.index

# INVALID INDEX (NON-EXISTENT PATH)
./indexer ../data/crawler/letters-1 ../data/nonExistentPath/letters-1.index

# INVALID INDEX (READ ONLY DIRECTORY)
mkdir ../data/indexer/unwriteableDir
chmod -w ../data/indexer/unwriteableDir
./indexer ../data/crawler/letters-1 ../data/indexer/unwriteableDir

# INVALID INDEX (READ ONLY FILE)
touch ../data/indexer/unwriteableFile
chmod -w ../data/indexer/unwriteableFile
./indexer ../data/crawler/letters-1 ../data/indexer/unwriteableFile

# TEST INDEXER (letters-1)
./indexer ../data/crawler/letters-1 ../data/indexer/letters-1.index

# TEST INDEXER (letters-2)
./indexer ../data/crawler/letters-2 ../data/indexer/letters-2.index

# TEST INDEXER (letters-3)
./indexer ../data/crawler/letters-3 ../data/indexer/letters-3.index

# TEST INDEXER (letters-10)
./indexer ../data/crawler/letters-10 ../data/indexer/letters-10.index

# TEST INDEXER (toscrape-0)
./indexer ../data/crawler/toscrape-0 ../data/indexer/toscrape-0.index

# TEST INDEXER (wiki-0)
./indexer ../data/crawler/wiki-0 ../data/indexer/wiki-0.index

# TEST INDEXTEST (letters-2-copy)
./indextest ../data/indexer/letters-2.index ../data/indexer/letters-2-copy.index

# TEST INDEXTEST (toscrape-0-copy)
./indextest ../data/indexer/toscrape-0.index ../data/indexer/toscrape-0-copy.index

exit 0