# ArduinoSudoku
This project is based upon the methodology presented by Peter Norvig an his post "Solving Every Sudoku Puzzle" https://norvig.com/sudoku.html

Two implementations of a Sudoku solver to run on ARM Arduinos. Tested on an Arduino Due with the WiFi version intended to run on a MKR WiFi 1010.

The SudokuSmall version runs on an Arduino Mega 2560 although with just a 16KHtz processor some solve times are extended. This version will load on a Uno but only the first two puzzles in the test suite (rated easy and medium) will run. Hard puzzles run out of memory.

Lengthy blog post on the trials and tribulations of building an Arduino C/C++ version can be found here. https://practicalarduinoc.blogspot.com/2019/10/an-arduino-sudoku-solver.html
