#include <hunspell.hxx>
#include <Rcpp.h>

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
    for (int j = 0; j < ns; j++)
      suggestions.push_back(wlst[j]);
    pMS->free_list(&wlst, ns);
    out.push_back(suggestions);
  }
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(std::string affix, CharacterVector dict, CharacterVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector pieces;
    int ns = pMS->analyze(&wlst, words[i]);
    for (int j = 0; j < ns; j++)
      pieces.push_back(wlst[j]);
    pMS->free_list(&wlst, ns);
    out.push_back(pieces);
  }
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(std::string affix, CharacterVector dict, CharacterVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector pieces;
    int ns = pMS->stem(&wlst, words[i]);
    for (int j = 0; j < ns; j++)
      pieces.push_back(wlst[j]);
    pMS->free_list(&wlst, ns);
    out.push_back(pieces);
  }
  delete pMS;
  return out;
}
