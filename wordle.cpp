#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

void helper(std::set<std::string>& words, 
  const std::set<std::string>& dict, 
  const std::string& original, 
  std::string str, 
  std::vector<char> floating, 
  unsigned int idx);

int dashCounter(const std::string& original, std::string str);

// Add prototypes of helper functions here
// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    std::set<std::string> words;
    std::string str = "";

		std::vector<char> sfloating;
    for (unsigned int i = 0; i < floating.size(); i++){
      sfloating.push_back(floating[i]);
		}
    
		helper(words, dict, in, str, sfloating, 0);

		return words;

}

// Define any helper functions here


// void helper func 
// (params: refence to set, string, alphabet, index in string)
void helper(std::set<std::string>& words, 
  const std::set<std::string>& dict, 
  const std::string& original, 
  std::string str, 
  std::vector<char> floating, 
  unsigned int idx)
{
// base case: if at the end of in word
	if (idx == original.size()){
		// check if in dictionary
		// if in dict, add to set, and return nothing!
		if (dict.find(str) != dict.end()){
			words.insert(str);
		}
		return;
	}
  if (original[idx] == '-'){
    // if "-" at that index, change
    // first, handle floating chars
    // if number of dashes < the number of floating, early return
    int dashes = dashCounter(original, str);
    if (dashes >= (int) floating.size()){
      for (int i = 0; i < (int) floating.size(); i++){
        str += (floating[i]);
        std::vector<char> floating_cpy(floating);
        floating_cpy.erase(floating_cpy.begin() + i);
        helper(words, dict, original, str, floating_cpy, idx + 1);

        // "undo"
        str.erase(str.size()-1, str.size());
      }
    }
    if (dashes > (int) floating.size()){
      for (char i = 'a'; i <= 'z'; i++){
        // loop through alphabet
        // substitue temp (not reference!) variable
        // recurse
        str += i;
        helper(words, dict, original, str, floating, idx + 1);

        // "undo"
        str.erase(str.size()-1, str.size());
      }
    }
  }
  else{
		// if not "-",
		// add the char at the index (that is already given)
		// go to next index of in
    helper(words, dict, original, str + original[idx], floating, idx + 1);
  }
}


// helper()'s helper function
// calulates the number of dashes remaining
int dashCounter(const std::string& original, std::string str)
{
  int cnt = 0;
  for (unsigned int i = str.size(); i < original.size(); i++){
    if (original[i] == '-'){
      cnt += 1;
    }
  }
  return cnt;
}

