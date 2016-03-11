#include <hunspell.hxx>
#include <Rcpp.h>
#include "textparser.hxx"

using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector R_hunspell_find(std::string affix, CharacterVector dict, CharacterVector text, CharacterVector ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //find valid characters in this language
  const char * wordchars = pMS->get_wordchars(); //latin1
  TextParser * p = new TextParser(wordchars);

  //int wordchars_utf16_len;
  //unsigned short * wordchars_utf16 = pMS->get_wordchars_utf16(&wordchars_utf16_len); //utf8
  //TextParser *p = new TextParser(wordchars_utf16, wordchars_utf16_len);

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    pMS->add(ignore[i]);
  }

  CharacterVector out;
  char * token;
  for(int i = 0; i < text.length(); i++){
    p->put_line(text[i]);
    p->set_url_checking(1);
    while ((token=p->next_token())) {
      if(!pMS->spell(token))
        out.push_back(token);
      free(token);
    }
  }
  delete p;
  return out;
}
