void clearBuffer() {
  memset(buffer, 0, BUFF_SIZE);
  charCount = 0;
}
void sendHTML() {
  client << "HTTP/1.1 200 OK\n";
  client << "Content-Type: text/html\n";
  client << "Connection: close\n\n"; // extra linefeed important
  client << "<!DOCTYPE HTML>\n";
  client << "<html>\n<head>\n";
  client << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  client << "<title>Sudoku Solver</title>\n</head>\n";
  client << "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script>\n";
  client << "<script type=\"text/javascript\" src=\"http://code.jquery.com/color/jquery.color-2.1.2.js\"></script>\n";
  jsHeader();
  client << "<body>\n";
client << "    <div style=\"width: 100%; height: 100%; background-color: antiquewhite;\">\n";
client << "        <div style=\"text-align: center;\">\n";
client << "            <h1>Sudoku Solver</h1>\n";
client << "            <div id=\"dvTable\">\n";
client << "            </div>\n";
client << "            <br/>\n";
client << "            <div>\n";
client << "                <input type=\"button\" class=\"btn\" value=\"Clear Grid\" onclick=\"clearTable();\"/>\n";
client << "                <input type=\"button\" class=\"btn\" value=\"Set Test\" onclick=\"setTable(testers[tIndex]);\"/>\n";
client << "                <input type=\"button\" class=\"btn\" value=\"Arduino Solve\" onclick=\"solveGrid();\"/>\n";
client << "                <p id=\"pprompt\">\n";
client << "                    Enter a Sudoku puzzle into the grid and<br />\n";
client << "                    then click the \"Arduino Solve\" button.<br />\n";
client << "                </p>\n";
client << "            </div>\n";
client << "        </div>\n";
client << "    </div>\n";
jsMain();
client << "</body></html>\n";
}
