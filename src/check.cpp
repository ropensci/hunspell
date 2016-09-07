#include "hunspell_types.h"

using namespace Rcpp;

// [[Rcpp::export]]
DictPtr R_hunspell_dict(Rcpp::String affix, CharacterVector dict){
  hunspell_dict *mydict = new hunspell_dict(affix, dict);
  return DictPtr(mydict);
}

// [[Rcpp::export]]
List R_hunspell_info(DictPtr ptr){
  return List::create(
    _["dict"] = ptr->dicts(),
    _["affix"] = ptr->affix(),
    _["encoding"] = ptr->enc(),
    _["wordchars"] = ptr->r_wordchars()
  );
}

// [[Rcpp::export]]
LogicalVector R_hunspell_check(DictPtr ptr, StringVector words){
  //check all words
  LogicalVector out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(ptr->spell(words[i]));
  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(DictPtr ptr, StringVector words){
  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(ptr->suggest(words[i]));
  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(DictPtr ptr, StringVector words){
  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(ptr->analyze(words[i]));
  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(DictPtr ptr, StringVector words){
  List out;
  for(int i = 0; i < words.length(); i++)
    out.push_back(ptr->stem(words[i]));
  return out;
}
