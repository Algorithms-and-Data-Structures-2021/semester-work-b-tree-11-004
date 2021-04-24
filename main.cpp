#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {
  cout << "Time test:\n";
  string a = Parser::process_data();
  cout << a;
  return 0;
}