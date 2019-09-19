#define BUFF_SIZE 200
#define BUFF_LIMIT 199
#include <WiFiNINA.h>
#include "Secret.h"
#include "Sudoku.h"
#include <malloc.h>

extern char _end;
extern "C" char *sbrk(int i);

char buffer[BUFF_SIZE];
const long timeOut = 500;
unsigned long waitStart;
int charCount;
const char* mySSID = SECRET_SSID;
const char* myPass = SECRET_PASSWORD;
const char getRequest[] = "GET /";
WiFiServer server(80); // port 80 is standard
WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  Sudoku::init();
  connectToWiFi();
  server.begin(); // start the server
}
void connectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    while (WiFi.begin(mySSID, myPass) != WL_CONNECTED) {
      delay(500);
    }
    Serial << "WiFi connected\n";
    Serial << "Local IP: " << WiFi.localIP() << '\n';
  }
}
void serveAjax() {
  bool error = false;
  char puzzle[82];
  unsigned long tStart = millis();
  char* strt = strstr(buffer, "ajax/?");
  if(strt) {
    strt += 6;
    strncpy(puzzle, strt, 81);
    //Serial << puzzle << '\n';
    Sudoku::maxCount = Sudoku::totalInstances = 0;
    tStart = millis();
    Sudoku s(puzzle);
    if(!s.hasError()) {
      auto S = solve(&s);
      if(S) {
        String sp = S->toString();
        strcpy(puzzle, sp.c_str());
      } else {
        strncpy(puzzle, "No solution found", 81);
        error = true;
      }
    } else {
      // return error for bad puzzle
      error = true;
      strncpy(puzzle, "Invalid puzzle", 81);
    }
  } else {
    strncpy(puzzle, "Puzzle not found", 81);
    error = true;
  }
  formatJSON(puzzle, millis() - tStart, error);
  client << buffer << '\n'; // return JSON to client
  clearBuffer();
  //Serial << "Free memory: " << freeRam() << '\n';
}
void formatJSON(char* puzzle, int elapsed, bool error) {
  // insert JSON format response into buffer
  String s("{\"solved\": \"");
  s += String(puzzle);
  s += String("\", \"msecs\": ");
  s += String(elapsed);
  s += String(", \"max\": ");
  s += String(Sudoku::maxCount);
  s += String(", \"tot\": ");
  s += String(Sudoku::totalInstances);
  s += String(", \"error\": ");
  s += String((error ? "true" : "false"));
  s += String("}");
  strncpy(buffer, s.c_str(), BUFF_SIZE);
}
void loop() {
  client = server.available();
  if(client) {
    waitStart = millis();
    clearBuffer();
    while (client.connected()) {
      if(client.available()) {
        char c = client.read();
        if(c == '\n' || c == '\r'){
          if(strncmp(getRequest, buffer, 5) == 0) {
            switch(buffer[5]) {
              case ' ': // initial get request
                sendHTML();
                break;
              case 'a':
                serveAjax();
              default:
                Serial << buffer << '\n';
                break;
            }
          }
          break; // escape while loop to close connection
        } else {
          if(charCount <= BUFF_LIMIT) {
            buffer[charCount] = c;
            charCount++;
          }
        }
      } else {
        if((unsigned long)millis() - waitStart >= timeOut) {
          Serial << "Out of time\n";
          break;
        }
      }
    }
    client.flush();
    client.stop();
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
