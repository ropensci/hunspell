#include "utils.h"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_info(std::string affix, CharacterVector dict){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);
  return List::create(
    _["dict"] = dict,
    _["encoding"] = mydict.enc(),
    _["wordchars"] = mydict.r_wordchars()
  );
}

// [[Rcpp::export]]
LogicalVector R_hunspell_check(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  //check all words
  LogicalVector out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.spell(words[i]));
  return out;

}

// [[Rcpp::export]]
List R_hunspell_suggest(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.suggest(words[i]));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.analyze(words[i]));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.stem(words[i]));

  return out;
}
