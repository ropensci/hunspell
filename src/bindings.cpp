#include <hunspell.hxx>
#include <Rcpp.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace Rcpp;

// [[Rcpp::export]]
LogicalVector R_hunspell_check(std::string affix, CharacterVector dict, CharacterVector words, CharacterVector ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    pMS->add(ignore[i]);
  }

  //check all words
  LogicalVector out;
  for(int i = 0; i < words.length(); i++){
    out.push_back(pMS->spell(words[i]));
  }
  delete pMS;
  return out;
}

// [[Rcpp::export]]
CharacterVector R_hunspell_find(std::string affix, CharacterVector dict, CharacterVector text, CharacterVector ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    pMS->add(ignore[i]);
  }

  CharacterVector out;
  for(int i = 0; i < text.length(); i++){
    String line = text[i];
    std::string myText(line.get_cstring());
    std::istringstream iss(myText);
    std::string token;
    while (std::getline(iss, token, ' ')) {
      if(!pMS->spell(token.c_str()))
        out.push_back(token);
    }
  }
  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(std::string affix, CharacterVector dict, CharacterVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector suggestions;
    int ns = pMS->suggest(&wlst, words[i]);
    for (int i = 0; i < ns; i++)
      suggestions.push_back(wlst[i]);
    pMS->free_list(&wlst, ns);
    out.push_back(suggestions);
  }
  delete pMS;
  return out;
}
