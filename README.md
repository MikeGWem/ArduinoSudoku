# ArduinoSudoku
Two implementations of a Sudoku solver to run on ARM Arduinos. Tested on an Arduino Due with the WiFi version intended to run on a MKR WiFi 1010.

The SudokuSmall version runs on an Arduino Mega 2560 although with just a 16KHtz processor some solve times are extended. This version will load on a Uno but only the first two puzzles in the test suite (rated easy and medium) will run. Hard puzzles run out of memory.

