#include "Puzzles.h"
#include "Sudoku.h"

char buffer[82];

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Sudoku::init(); //call static method
  //strcpy_P(buffer, (char*)pgm_read_word(&(pList[7])));
  //solvePuzzle(buffer, true);
  runTests();
}
void runTests() {
  for(int i = 0, j =  95; i < j; i++) {
    strcpy_P(buffer, (char*)pgm_read_word(&(pList[i])));
    float elaps = solvePuzzle(buffer, false);
    Serial << F("Puzzle P") << (i+1) << F(" solved in ");
    Serial.print(elaps, 3);
    Serial << F(" seconds\n");
  }
}
float solvePuzzle(char* p, bool show) {
  unsigned long strt = millis();
  Sudoku::maxCount = Sudoku::totalInstances = 0;
  Sudoku s(p);
  auto S = solve(&s);
  if(S) {
    float tmr = (millis() - strt) / 1000.0;
    if(show) {
      Serial << F("Solved in ");
      Serial.print(tmr, 3);
      Serial << F(" seconds\n");
      Serial << F("Peak instaces ") << Sudoku::maxCount << F(" total instances ") << Sudoku::totalInstances << F(".\n\n");
      S->write();
    }
    return tmr;
    //keep toString() for later
  } else {
    if(show) {
     Serial << F("Solution not found\n"); 
    }
    return -1;
  }  
}

void loop() {
  // put your main code here, to run repeatedly:

}
