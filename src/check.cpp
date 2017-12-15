#include "hunspell_types.h"

using namespace Rcpp;

// [[Rcpp::export]]
DictPtr R_hunspell_dict(Rcpp::String affix, CharacterVector dict, StringVector add_words){
  hunspell_dict *mydict = new hunspell_dict(affix, dict, add_words);
  return DictPtr(mydict);
}

// [[Rcpp::export]]
List R_hunspell_info(DictPtr ptr){
  return List::create(
    _["dict"] = ptr->dicts(),
    _["affix"] = ptr->affix(),
    _["encoding"] = ptr->enc(),
    _["wordchars"] = ptr->r_wordchars(),
    _["added"] = ptr->added()
  );
}

// [[Rcpp::export]]
LogicalVector R_hunspell_check(DictPtr ptr, StringVector words){
  //check all words
  int len = words.length();
  LogicalVector out(len);
  for(int i = 0; i < len; i++)
    out[i] = StringVector::is_na(words[i]) ? NA_LOGICAL : ptr->spell(words[i]);
  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(DictPtr ptr, StringVector words){
  int len = words.length();
  List out(len);
  for(int i = 0; i < len; i++)
    if(!StringVector::is_na(words[i]))
      out[i] = ptr->suggest(words[i]);
  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(DictPtr ptr, StringVector words){
  int len = words.length();
  List out(len);
  for(int i = 0; i < len; i++)
    if(!StringVector::is_na(words[i]))
      out[i] = ptr->analyze(words[i]);
  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(DictPtr ptr, StringVector words){
  int len = words.length();
  List out(len);
  for(int i = 0; i < len; i++)
    if(!StringVector::is_na(words[i]))
      out[i] = ptr->stem(words[i]);
  return out;
}
