# define P_SIZE 81

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

class Possible {
  protected:
  int16_t values;
  public:
  Possible() {
    values = -1; // sets all bits to 1 (true)
  }
  bool isSet(uint8_t t) {
    return bitRead(values, t-1);
  }
  void eliminate(uint8_t t) {
    bitClear(values, t-1);
  }
  uint8_t count() {
    uint8_t c = 0;
    for(int i = 0; i < 9; i++){
      if(bitRead(values, i)) {c++;}
    }
    return c;
  }
  uint8_t value() {
    for(int i = 0; i < 9; i++){
      if(bitRead(values, i)) {return i+1;}
    }
    return 0;
  }
  Possible clone() {
    Possible p;
    p.values = this->values;
    return p;
  }
  void str(uint8_t, char*);
};
void Possible::str(uint8_t width, char* c) {
  memset(c, ' ', width);
  int8_t pos=0;
  for(int i = 1; i<= 9; i++) {
    if(isSet(i)) {c[pos++] = '0'+i;}
  }
}

class Sudoku {
  Possible cells[81];
  static uint8_t squares[9][9];
  static uint8_t rows[9][9];
  static uint8_t cols[9][9];
  bool eliminate(uint8_t, uint8_t);
  uint8_t getRow(uint8_t);
  uint8_t getCol(uint8_t);
  uint8_t getSquare(uint8_t);
  bool puzzleError;
public:
  static int32_t maxCount, curInstances, totalInstances;
  static void init();
  Sudoku(const char*);
  Sudoku(Sudoku*);
  ~Sudoku();
  bool isSolved();
  bool hasError() {return puzzleError;}
  uint8_t leastCount();
  bool assign(uint8_t, uint8_t);
  String toString();
  void write();
  Possible possible(uint8_t p) {
    return cells[p];
  }
};
// static data members declared
uint8_t Sudoku::squares[9][9];
uint8_t Sudoku::rows[9][9];
uint8_t Sudoku::cols[9][9];
// plus some counts for fun and interest
int32_t Sudoku::maxCount, Sudoku::curInstances, Sudoku::totalInstances;

Sudoku::Sudoku(const char* p){
  curInstances++;
  totalInstances++;
  maxCount = max(maxCount, curInstances);
  puzzleError = false;
  for(int i = 0; i < P_SIZE; i++) {
    Possible p;
    cells[i] = p;
  }
  int cNum = 0;
  for(int i = 0, j = strlen(p); i < j; i++) {
    char c = p[i];
    if(c >= '1' && c <= '9') {
      if(!assign(cNum, c-48)) {
        puzzleError = true; // signals invalid puzzle
        //Serial << "Error in puzzle\n";
      }
    }
    cNum++;
  }
}
Sudoku::Sudoku(Sudoku* s) {
  curInstances++;
  totalInstances++;
  maxCount = max(maxCount, curInstances);
  for (int i = 0; i < P_SIZE; i++) {
    Possible p = s->cells[i].clone();
    cells[i] = p;
  }
}
Sudoku::~Sudoku() {
  curInstances--;
}
bool Sudoku::assign(uint8_t cell, uint8_t val) {
  for(int i = 1; i <= 9; i++) {
    if(i != val) {
      // unset the other values in cell Possible instance
      if(!eliminate(cell, i)) { return false;}
    }
  }
  return true;
}
bool Sudoku::eliminate(uint8_t cell, uint8_t val) {
  if(!cells[cell].isSet(val)) {return true;} // not set
  cells[cell].eliminate(val); // unset
  uint8_t vCount = cells[cell].count(); // how many possibles left?
  if(vCount == 0) {
    return false; // woops
  }
  if(vCount == 1) {
    // now a single value so must eliminate from shared row, columnd and square
    uint8_t rv = cells[cell].value();
    uint8_t group = getRow(cell);
    for(int i = 0; i < 9; i++) {
      if(rows[group][i] != cell) {
        if(!eliminate(rows[group][i], rv)) { return false;}
      }
    }
    group = getCol(cell);
    for(int i = 0; i < 9; i++) {
      if(cols[group][i] != cell) {
        if(!eliminate(cols[group][i], rv)) { return false;}
      }
    }
    group = getSquare(cell);
    for(int i = 0; i < 9; i++) {
      if(squares[group][i] != cell) {
        if(!eliminate(squares[group][i], rv)) { return false;}
      }
    }
    return true;
  }
}
bool Sudoku::isSolved() {
  for(int i = 0; i < P_SIZE; i++) {
    if(cells[i].count() != 1) {
      return false;
    }
  }
  return true;
}
String Sudoku::toString() {
  String s;
  s.reserve(P_SIZE);
  for(int i = 0; i < P_SIZE; i++) {
    s+= String(cells[i].value());
  }
  return s;
}
// returns first unresolved cell with the fewest possible values
uint8_t Sudoku::leastCount() {
   int8_t cell = -1, min; // char not byte
   for (int i = 0; i < P_SIZE; i++) {
      uint8_t m = cells[i].count();
      if (m > 1 && (cell == -1 || m < min)) {
         min = m, cell = i;
      }
   }
   return cell;
}
Sudoku* solve(Sudoku* S) {
  if(S->isSolved()) {return S;}
  uint8_t cell = S->leastCount(); // get cell with least possibles
  Possible p = S->possible(cell);
  for(int i = 1; i <= 9; i++) {
    if(p.isSet(i)) {
      // we need to create a copy of the Sudoku instance
      Sudoku n(S);
      // assign the selected value in that instance
      if(n.assign(cell,i)){
        // and call solve on that
        if(auto S2 = solve(&n)) {
          return S2;
        }
      }
      // if not solved then use the next possible value in that cell
    }
  }
  return NULL;
}
// write() can also be used as a disgnostic
// aid to display a part solved puzzle
// thanks Pau Fernandez
//void Sudoku::write() {
//  uint8_t width = 1;
//  for(int i = 0; i < P_SIZE; i++) {
//    width = max(width, cells[i].count());
//  }
//  uint8_t fw = 3 * width + 1;
//  char sep[fw], vals[fw];
//  memset(sep, 0, fw);
//  memset(vals, 0, fw);
//  memset(sep, '-', fw - 1);
//  for(int i = 0; i < 9; i++) {
//    if(i == 3 || i == 6) {
//      Serial << sep << "+-" << sep << "+" << sep << "-\n";
//    }
//    for( int j = 0; j < 9; j++) {
//      if(j == 3 || j == 6) {Serial << "| ";}
//      cells[i * 9 + j].str(width, vals);
//      Serial << vals;
//    }
//    Serial << '\n';
//  }
//}
void Sudoku::init() {
  //Build look-up tables for rows, columns and squares
  for(int i = 0; i < 9; i++) {
    int sb = (i / 3) * 27 + (i % 3) * 3;
    for(int j = 0; j< 9; j++) {
      cols[i][j] = j * 9 + i;
      rows[i][j] = i * 9 + j;
      squares[i][j] = sb + j + (j / 3) * 6;
    }
  }
  maxCount = curInstances, totalInstances = 0;
}
uint8_t Sudoku::getRow(uint8_t cell) {
  return cell/9;
}
uint8_t Sudoku::getCol(uint8_t cell) {
  return cell % 9;
}
uint8_t Sudoku::getSquare(uint8_t cell) {
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 9; j++) {
      if(squares[i][j] == cell) {
        return i;
      }
    }
  }
}
