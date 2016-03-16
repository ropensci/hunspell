#include <hunspell.hxx>
#include <Rcpp.h>
#include "convert.h"

#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_find(std::string affix, std::string dict, StringVector text,
                     StringVector ignore, std::string format){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict.c_str());
  if(!pMS)
    throw std::runtime_error(std::string("Failed to load ") + dict.c_str());
  char * enc = pMS->get_dic_encoding();
  iconv_t cd_from = iconv_from_r(enc);
  iconv_t cd_to = iconv_to_r(enc);

  //int wordchars_utf16_len;
  //unsigned short * wordchars_utf16 = pMS->get_wordchars_utf16(&wordchars_utf16_len); //utf8
  //TextParser *p = new TextParser(wordchars_utf16, wordchars_utf16_len);

  //find valid characters in this language
  const char * wordchars = pMS->get_wordchars(); // 8bit encodings, e.g. latin1 or similar
  int utf16_len;
  unsigned short * ustf16_wordchars = pMS->get_wordchars_utf16(&utf16_len);
  TextParser * p = NULL;
  if(strcmp(enc, "UTF-8") == 0){
    if(!format.compare("text")){
      p = new TextParser(ustf16_wordchars, utf16_len);
    } else if(!format.compare("latex")){
      p = new LaTeXParser(ustf16_wordchars, utf16_len);
    } else if(!format.compare("man")){
      p = new ManParser(ustf16_wordchars, utf16_len);
    } else {
      throw std::runtime_error("Unknown parse format");
    }
  } else {
    if(!format.compare("text")){
      p = new TextParser(wordchars);
    } else if(!format.compare("latex")){
      p = new LaTeXParser(wordchars);
    } else if(!format.compare("man")){
      p = new ManParser(wordchars);
    } else {
      throw std::runtime_error("Unknown parse format");
    }
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    char * str = string_from_r(ignore[i], cd_from);
    if(str != NULL){
      pMS->add(str);
      free(str);
    }
  }

  List out;
  char * token;
  for(int i = 0; i < text.length(); i++){
    CharacterVector words;
    char * str = string_from_r(text[i], cd_from);
    if(str == NULL){
      Rf_warningcall(R_NilValue, "Failed to convert line %d to %s encoding. Cannot spell check with this dictionary.", i + 1, enc);
    } else {
      p->put_line(str);
      p->set_url_checking(1);
      while ((token=p->next_token())) {
        if(!pMS->spell(token))
          words.push_back(string_to_r(token, cd_to));
        free(token);
      }
      free(str);
    }
    out.push_back(words);
  }
  delete p;
  delete pMS;
  return out;
}
