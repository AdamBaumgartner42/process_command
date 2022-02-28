#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// -std=c++17 

vector<string> splitOnString(string &input, string delimiter){
  vector<string> result;
  int start = 0;
  int end = input.substr(0).find(delimiter); // begin .find() at index 0 of the string
  while(start <= end) {
    string word = input.substr(start, end-start); // end - start is length
    result.push_back(word);
    start = end + delimiter.size(); // update start index
    end = start + input.substr(start).find(delimiter); // update end index
  }
  string word = input.substr(start);
  result.push_back(word);
  return result;
}

string parse_delimiter (ifstream& filestream, string delimiter){
  string cmd;
  for(string line; getline(filestream,line);){
    vector<string> info = splitOnString(line, delimiter);
    for(auto data : info){ // issue with first space included
      cmd = cmd + delimiter + data;
    }
  }
  return cmd;
}

// Problem: .c_str() is incorretly shortening the name of the initial command

// file: ncurses_display.cpp 
/*
mvwprintw(window, row, command_column, processes[i].Command().substr(0, window->_maxx - 46).c_str();
*/
// note: .c_str() is causing the shortening error.

// file: process.cpp
/*
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}
*/

// file: linux_parser.cpps
/*
string LinuxParser::Command(int pid) { 
  std::ifstream filestream("/proc/" + std::to_string(pid) + "/cmdline");
  string line;
  if(getline(filestream, line)){
    return line;
  } else {
    return string();
  }
}
*/

// Demonstration of the problem
int main () {
  int pid = 1;
  ifstream filestream("/proc/" + std::to_string(pid) + "/cmdline");
  string line;
  getline(filestream, line);
  cout << line << "\n";
  cout << line.substr(0, 50) << "\n";
  cout << line.substr(0, 50).c_str() << "\n";
  return 0;
}
