#include <hunspell.hxx>
#include <Rcpp.h>
#include "convert.h"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_info(std::string affix, CharacterVector dict){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_to_r_t cd = iconv_to_r(enc);
  List out = List::create(
    _["dict"] = dict,
    _["encoding"] = CharacterVector(enc),
    _["wordchars"] = CharacterVector(string_to_r((char *) pMS->get_wordchars(), cd, enc))
  );
  iconv_close(cd);
  delete pMS;
  return out;
}

// [[Rcpp::export]]
LogicalVector R_hunspell_check(std::string affix, CharacterVector dict, StringVector words, StringVector ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_from_r_t cd = iconv_from_r(enc);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    char * str = string_from_r(ignore[i], cd, enc);
    pMS->add(str);
    free(str);
  }

  //check all words
  LogicalVector out;
  for(int i = 0; i < words.length(); i++){
    char * str = string_from_r(words[i], cd, enc);
    out.push_back(pMS->spell(str));
    free(str);
  }
  iconv_close(cd);
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_suggest(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_from_r_t cd_from = iconv_from_r(enc);
  iconv_to_r_t cd_to = iconv_to_r(enc);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector suggestions;
    char * str = string_from_r(words[i], cd_from, enc);
    int ns = pMS->suggest(&wlst, str);
    free(str);
    for (int j = 0; j < ns; j++)
      suggestions.push_back(string_to_r(wlst[j], cd_to, enc));
    pMS->free_list(&wlst, ns);
    out.push_back(suggestions);
  }
  iconv_close(cd_from);
  iconv_close(cd_to);
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_analyze(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_from_r_t cd_from = iconv_from_r(enc);
  iconv_to_r_t cd_to = iconv_to_r(enc);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector pieces;
    char * str = string_from_r(words[i], cd_from, enc);
    int ns = pMS->analyze(&wlst, str);
    free(str);
    for (int j = 0; j < ns; j++)
      pieces.push_back(string_to_r(wlst[j], cd_to, enc));
    pMS->free_list(&wlst, ns);
    out.push_back(pieces);
  }
  iconv_close(cd_from);
  iconv_close(cd_to);
  delete pMS;
  return out;
}

// [[Rcpp::export]]
List R_hunspell_stem(std::string affix, CharacterVector dict, StringVector words){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_from_r_t cd_from = iconv_from_r(enc);
  iconv_to_r_t cd_to = iconv_to_r(enc);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  List out;
  char ** wlst;
  for(int i = 0; i < words.length(); i++){
    CharacterVector pieces;
    char * str = string_from_r(words[i], cd_from, enc);
    int ns = pMS->stem(&wlst, str);
    free(str);
    for (int j = 0; j < ns; j++)
      pieces.push_back(string_to_r(wlst[j], cd_to, enc));
    pMS->free_list(&wlst, ns);
    out.push_back(pieces);
  }
  iconv_close(cd_from);
  iconv_close(cd_to);
  delete pMS;
  return out;
}
