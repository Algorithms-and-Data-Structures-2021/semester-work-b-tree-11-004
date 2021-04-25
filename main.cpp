#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {

  string pathToTests;
  string pathToResult;
  int iterations = 1;


  cout << "Enter path to file with tests:\n";
  cin >> pathToTests;
  cout << "Enter path to results file:\n";
  cin >> pathToResult;

  cout << "Enter iterations:\n";
  cin >> iterations;

  Parser::process_data(pathToTests, pathToResult, iterations);
  return 0;
}