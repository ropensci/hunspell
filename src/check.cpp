#include <hunspell.hxx>
#include <Rcpp.h>
#include <iconv.h>
#include <errno.h>

using namespace Rcpp;

char * r_to_string(String str, char * enc){
  iconv_t cd = iconv_open(str.get_encoding().c_str(), enc);
}

String string_to_r(char * inbuf, char * enc){
  iconv_t cd = iconv_open("UTF-8", enc);
  if(cd == (iconv_t) -1){
    iconv_close(cd);
    switch(errno){
      case EINVAL: std::runtime_error(std::string("Iconv does not support UTF8 conversion for ") + enc);
      default: std::runtime_error("General error in iconv_open()");
    }
  }
  size_t inlen = strlen(inbuf);
  size_t outlen = inlen * 4;
  char output[outlen];
  char * outp = output;
  size_t success = iconv(cd, &inbuf, &inlen, &outp, &outlen);
  if(success == (size_t) -1){
    iconv_close(cd);
    switch(errno){
      case E2BIG: std::runtime_error("Iconv insufficient memory");
      case EILSEQ: std::runtime_error("An invalid multibyte sequence has been encountered in the input.");
      case EINVAL: std::runtime_error("An incomplete multibyte sequence has been encountered in the input.");
      default: std::runtime_error("General error in iconv()");
    }
  }
  outp[0] = '\0';
  String res = String(output);
  res.set_encoding(CE_UTF8);
  return res;
}

// [[Rcpp::export]]
List R_hunspell_info(std::string affix, CharacterVector dict){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * wc = (char*) pMS->get_wordchars();
  char * enc = pMS->get_dic_encoding();

  List out = List::create(
    _["dict"] = dict,
    _["encoding"] = CharacterVector(enc),
    _["wordchars"] = CharacterVector(string_to_r(wc, enc))
  );
  delete pMS;
  return out;
}

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
