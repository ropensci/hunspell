#include <hunspell.hxx>
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector R_hunspell_check(std::string affix, CharacterVector dict, CharacterVector words, CharacterVector ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(size_t i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(size_t i = 0; i < ignore.length(); i++){
    pMS->add(ignore[i]);
  }

  //check all words
  CharacterVector out;
  for(int i = 0; i < words.length(); i++){
    if(!pMS->spell(words[i])){
      out.push_back(words[i]);
    }
  }
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(std::string affix, CharacterVector dict, CharacterVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //add additional dictionaries if more than one
  for(size_t i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector suggestions;
    if(pMS->spell(words[i])){
      suggestions.push_back(words[i]);
    } else {
      int ns = pMS->suggest(&wlst, words[i]);
      for (int i = 0; i < ns; i++)
        suggestions.push_back(wlst[i]);
      pMS->free_list(&wlst, ns);
    }
    out.push_back(suggestions);
  }
  delete pMS;
  return out;
}
