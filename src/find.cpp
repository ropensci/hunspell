#include <hunspell.hxx>
#include <Rcpp.h>
#include "convert.h"

#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_find(std::string affix, CharacterVector dict, StringVector text,
                     StringVector ignore, std::string format){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);
  char * enc = pMS->get_dic_encoding();
  iconv_t cd_from = iconv_from_r(enc);

  //this doesn't work?
  //int wordchars_utf16_len;
  //unsigned short * wordchars_utf16 = pMS->get_wordchars_utf16(&wordchars_utf16_len); //utf8

  //find valid characters in this language
  size_t wordchars_utf16_len = 0;
  const char * wordchars = pMS->get_wordchars(); // dictionary 8bit encoding, e.g. latin1
  unsigned short * wordchars_utf16 = string_to_utf16((char*) wordchars, enc, &wordchars_utf16_len);

  TextParser * p = NULL;
  if(!format.compare("text")){
    p = new TextParser(wordchars_utf16, wordchars_utf16_len);
  } else if(!format.compare("latex")){
    p = new LaTeXParser(wordchars_utf16, wordchars_utf16_len);
  } else if(!format.compare("man")){
    p = new ManParser(wordchars_utf16, wordchars_utf16_len);
  } else {
    throw std::runtime_error("Unknown parse format");
  }

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
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
    String line = text[i];
    line.set_encoding(CE_UTF8);
    p->put_line((char*)line.get_cstring());
    p->set_url_checking(1);
    while ((token=p->next_token())) {
      //try to convert word from UTF8 to dictionary 8bit encoding
      char * str = string_convert(token, cd_from);
      if(!str || !pMS->spell(str)){
        String x = String(str);
        x.set_encoding(CE_UTF8);
        words.push_back(x);
        //words.push_back(string_to_r(token, cd_to));
      }
      free(token);
      free(str);
    }
    out.push_back(words);
  }
  delete p;
  delete pMS;
  return out;
}
