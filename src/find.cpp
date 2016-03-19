#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"

#include "utils.h"

using namespace Rcpp;

class hunspell_parser {
  TextParser *parser;
  hunspell_dict *mydict;
  unsigned short * utf16_wc;
  int utf16_len;

public:
  hunspell_parser(hunspell_dict *mydict, std::string format) : mydict(mydict) {
    utf16_wc = mydict->get_wordchars_utf16(&utf16_len);
    if(strcmp(mydict->enc(), "UTF-8") == 0){
      if(!format.compare("text")){
        parser = new TextParser(utf16_wc, utf16_len);
      } else if(!format.compare("latex")){
        parser = new LaTeXParser(utf16_wc, utf16_len);
      } else if(!format.compare("man")){
        parser = new ManParser(utf16_wc, utf16_len);
      } else {
        throw std::runtime_error("Unknown parse format");
      }
    } else {
      // 8bit encodings, e.g. latin1 or similar
      if(!format.compare("text")){
        parser = new TextParser(mydict->wc());
      } else if(!format.compare("latex")){
        parser = new LaTeXParser(mydict->wc());
      } else if(!format.compare("man")){
        parser = new ManParser(mydict->wc());
      } else {
        throw std::runtime_error("Unknown parse format");
      }
    }
  }

  ~hunspell_parser() {
    try {
      delete parser;
    } catch (...) {}
  }

  CharacterVector parse(String txt){
    char * token;
    CharacterVector output;
    txt.set_encoding(CE_UTF8);
    parser->put_line((char*) txt.get_cstring());
    parser->set_url_checking(1);
    while ((token=parser->next_token())) {
      String x(token);
      x.set_encoding(CE_UTF8);
      output.push_back(x);
      free(token);
    }
    return output;
  }

  CharacterVector check(String txt, int i){
    CharacterVector words;
    char * token;
    char * str = mydict->string_from_r(txt);
    if(str == NULL){
      Rf_warningcall(R_NilValue, "Failed to convert line %d to %s encoding. Cannot spell check with this dictionary. Try using a UTF8 dictionary.", i + 1, mydict->enc());
    } else {
      parser->put_line(str);
      parser->set_url_checking(1);
      while ((token=parser->next_token())) {
        if(!mydict->spell_char(token))
          words.push_back(mydict->string_to_r(token));
        free(token);
      }
      free(str);
    }
    return words;
  }
};

// [[Rcpp::export]]
List R_hunspell_find(std::string affix, std::string dict, StringVector text,
                     std::string format, StringVector ignore){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);
  hunspell_parser p(&mydict, format);

  //add ignore words
  mydict.add_words(ignore);

  List out;
  for(int i = 0; i < text.length(); i++)
    out.push_back(p.check(text[i], i));

  return out;
}

// [[Rcpp::export]]
List R_hunspell_parse(std::string affix, std::string dict, StringVector text,
                     std::string format){

  //init with affix and at least one dict
  hunspell_dict mydict(affix, dict);
  hunspell_parser p(&mydict, format);

  List out;
  for(int i = 0; i < text.length(); i++)
    out.push_back(p.parse(text[i]));

  return out;
}
