#include <hunspell.hxx>
#include <Rcpp.h>

#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"

using namespace Rcpp;

// [[Rcpp::export]]
List R_hunspell_find(std::string affix, CharacterVector dict, CharacterVector text,
                                CharacterVector ignore, std::string format){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(affix.c_str(), dict[0]);

  //int wordchars_utf16_len;
  //unsigned short * wordchars_utf16 = pMS->get_wordchars_utf16(&wordchars_utf16_len); //utf8
  //TextParser *p = new TextParser(wordchars_utf16, wordchars_utf16_len);

  //find valid characters in this language
  const char * wordchars = pMS->get_wordchars(); //latin1
  TextParser * p = NULL;
  if(!format.compare("text")){
    p = new TextParser(wordchars);
  } else if(!format.compare("latex")){
    p = new LaTeXParser(wordchars);
  } else if(!format.compare("man")){
    p = new ManParser(wordchars);
  } else {
    throw std::runtime_error("Unknown parse format");
  }

  //add additional dictionaries if more than one
  for(int i = 1; i < dict.length(); i++){
    pMS->add_dic(dict[i]);
  }

  //add ignore words
  for(int i = 0; i < ignore.length(); i++){
    pMS->add(ignore[i]);
  }

  List out;
  char * token;
  for(int i = 0; i < text.length(); i++){
    CharacterVector words;
    p->put_line(text[i]);
    p->set_url_checking(1);
    while ((token=p->next_token())) {
      if(!pMS->spell(token))
        words.push_back(token);
      free(token);
    }
    out.push_back(words);
  }
  delete p;
  delete pMS;
  return out;
}
