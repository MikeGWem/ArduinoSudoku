#include "Puzzles.h"
#include "Sudoku.h"
#include <malloc.h>

float solvePuzzle(char*, bool = false);
//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
// next two lines needed by freeRam() on Due
extern char _end;
extern "C" char *sbrk(int i);


void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Serial << "Free memory: " << freeRam() << '\n';
  Sudoku::init(); //call static method
  solvePuzzle((char*)P8, true);
  //runTests();
  Serial << "Instances now: " << Sudoku::curInstances << '\n';
  Serial << "Free memory: " << freeRam() << '\n';
}
void runTests() {
  for(int i = 0, j =  sizeof(pList) / sizeof(char*); i < j; i++) {
    float elaps = solvePuzzle((char*)pList[i], false);
    Serial << "Puzzle P" << (i+1) << " solved in ";
    Serial.print(elaps, 3);
    Serial << " seconds\n";
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
      Serial << "Solved in ";
      Serial.print(tmr, 3);
      Serial << " seconds\n";
      Serial << "Peak instaces " << Sudoku::maxCount << " total instances " << Sudoku::totalInstances << ".\n\n";
      S->write();
    }
    return tmr;
    //keep toString() for later
  } else {
    if(show) {
     Serial << "Solution not found\n"; 
    }
    return -1;
  }  
}
int freeRam() {
  char *ramstart=(char *)0x20070000;
  char *ramend=(char *)0x20088000;
  char *heapend=sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi=mallinfo();
  return stack_ptr - heapend + mi.fordblks;
  // mi.uordblks would return ram used
  // ramend - stack_ptr would return stack size
}

void loop() {
  // put your main code here, to run repeatedly:

}
