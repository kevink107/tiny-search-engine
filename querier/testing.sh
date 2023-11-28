#!/bin/bash
# Kevin King
# Querier Test
# CS50 Winter 2022

export tseoutput="/thayerfs/home/f004xn4/cs50-dev/shared/tse/output"

## INVALID ARGUMENTS

# no arguments
./querier 

# 1 argument
./querier x

# 3 arguments
./querier x y z

# not crawler directory
mkdir notCrawlerDir
./querier notCrawlerDir indexFilename

# non-existent directory
./querier nonExistentDir indexFilename

# invalid indexFilename
./querier $tseoutput/crawler/pages-letters-depth-1 invalidIndexFilename

# invalid characters
echo abc123 > testFile
./querier $tseoutput/crawler/pages-letters-depth-1 $tseoutput/letters-1.index < testFile

# 'and' at the beginning
echo and query > testFile
./querier $tseoutput/crawler/pages-letters-depth-1 $tseoutput/letters-1.index < testFile

# 'or' at the end
echo query or > testFile
./querier $tseoutput/crawler/pages-letters-depth-1 $tseoutput/letters-1.index < testFile

# consecutive 'ands'
echo hi and and there > testFile
./querier $tseoutput/crawler/pages-letters-depth-1 $tseoutput/letters-1.index < testFile

## TESTING WITH SHARED FILES
# using crawler and index output files from TSE shared output folder 
# path: /thayerfs/home/f004xn4/cs50-dev/shared/tse/output
# test different queries using fuzz testing for random sets of data
# make sure to 'make all' so that fuzzquery is compiled before running

# test letters at depth 10 using own test input
# this test file does not include spaces at the end of each line
./querier $tseoutput/crawler/pages-letters-depth-10 $tseoutput/letters-10.index < testQuery

# test letters at depth 0
./fuzzquery $tseoutput/letters-0.index 5 1 > testQueryOutput
./querier $tseoutput/crawler/pages-letters-depth-0 $tseoutput/letters-0.index < testQueryOutput

# test letters at depth 0 - tries a different random set
./fuzzquery $tseoutput/letters-0.index 5 2 > testQueryOutput
./querier $tseoutput/crawler/pages-letters-depth-0 $tseoutput/letters-0.index < testQueryOutput

# test letters at depth 3
./fuzzquery $tseoutput/letters-3.index 5 5 > testQueryOutput
./querier $tseoutput/crawler/pages-letters-depth-3 $tseoutput/letters-3.index < testQueryOutput

# test to-scrape at depth 1
./fuzzquery $tseoutput/toscrape-1.index 5 2 > testQueryOutput
./querier $tseoutput/crawler/pages-toscrape-depth-1 $tseoutput/toscrape-1.index < testQueryOutput

## MEMORY LEAK TESTING WITH VALGRIND

# valgrind for fuzzquery
./fuzzquery $tseoutput/letters-3.index 5 5 > testQueryOutput
valgrind --leak-check=full --show-leak-kinds=all ./querier $tseoutput/crawler/pages-letters-depth-3 $tseoutput/letters-3.index < testQueryOutput

# valgrind for own test input 
valgrind --leak-check=full --show-leak-kinds=all ./querier $tseoutput/crawler/pages-letters-depth-10 $tseoutput/letters-10.index < testQuery

rm -rf notCrawlerDir
rm -rf testFile
rm -rf testQueryOutput

exit 0
