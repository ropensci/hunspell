#include "utils.h"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_info(std::string affix, std::string dict){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);
  return List::create(
    _["dict"] = dict,
    _["encoding"] = CharacterVector(mydict.enc()),
    _["wordchars"] = mydict.string_to_r(mydict.wc())
  );
}

// [[Rcpp::export]]
LogicalVector R_hunspell_check(std::string affix, std::string dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  //check all words
  LogicalVector out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.spell(words[i]));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(std::string affix, std::string dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.suggest(words[i]));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(std::string affix, std::string dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.analyze(words[i]));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(std::string affix, std::string dict, StringVector words){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(mydict.stem(words[i]));

  return out;
}
