#include <hunspell.hxx>
#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"

#include "utils.h"

// [[Rcpp::export]]
List R_hunspell_find(std::string affix, std::string dict, StringVector text,
                     StringVector ignore, std::string format){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);

  //int wordchars_utf16_len;
  //unsigned short * wordchars_utf16 = pMS->get_wordchars_utf16(&wordchars_utf16_len); //utf8
  //TextParser *p = new TextParser(wordchars_utf16, wordchars_utf16_len);

  //find valid characters in this language
  int utf16_len;
  unsigned short * utf16_wc = mydict.get_wordchars_utf16(&utf16_len);
  TextParser * p = NULL;
  if(strcmp(mydict.enc(), "UTF-8") == 0){
    if(!format.compare("text")){
      p = new TextParser(utf16_wc, utf16_len);
    } else if(!format.compare("latex")){
      p = new LaTeXParser(utf16_wc, utf16_len);
    } else if(!format.compare("man")){
      p = new ManParser(utf16_wc, utf16_len);
    } else {
      throw std::runtime_error("Unknown parse format");
    }
  } else {
    // 8bit encodings, e.g. latin1 or similar
    if(!format.compare("text")){
      p = new TextParser(mydict.wc());
    } else if(!format.compare("latex")){
      p = new LaTeXParser(mydict.wc());
    } else if(!format.compare("man")){
      p = new ManParser(mydict.wc());
    } else {
      throw std::runtime_error("Unknown parse format");
    }
  }

  //add ignore words
  mydict.add_words(ignore);

  List out;
  char * token;
  for(int i = 0; i < text.length(); i++){
    CharacterVector words;
    char * str = mydict.string_from_r(text[i]);
    if(str == NULL){
      Rf_warningcall(R_NilValue, "Failed to convert line %d to %s encoding. Cannot spell check with this dictionary. Try using a UTF8 dictionary.", i + 1, mydict.enc());
    } else {
      p->put_line(str);
      p->set_url_checking(1);
      while ((token=p->next_token())) {
        if(!mydict.spell_char(token))
          words.push_back(mydict.string_to_r(token));
        free(token);
      }
      free(str);
    }
    out.push_back(words);
  }
  delete p;
  return out;
}
