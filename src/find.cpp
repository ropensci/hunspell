/* Note: The hunspell parser crashes for large strings. Therefore we first
 * split by whitespace before feeding to the parser, which tokenizes further
 * using characters and punctuation from the dictionary.
 * See also https://github.com/ropensci/hunspell/issues/5
 */

#include "parsers/textparser.hxx"
#include "parsers/latexparser.hxx"
#include "parsers/manparser.hxx"
#include "parsers/xmlparser.hxx"
#include "parsers/htmlparser.hxx"

#include "hunspell_types.h"

using namespace Rcpp;

class LIBHUNSPELL_DLL_EXPORTED hunspell_parser {
  TextParser *parser;
  hunspell_dict *mydict;
  const w_char* utf16_wc;
  int utf16_len;

public:
  hunspell_parser(hunspell_dict *mydict, std::string format) : mydict(mydict) {
    /* TO DO: should pass utf16_wc to the parser but this doesn't work
    const std::vector<w_char> vec_wordchars_utf16 = mydict->get_wordchars_utf16();
    utf16_len = vec_wordchars_utf16.size();
    utf16_wc = vec_wordchars_utf16.data();
    */

    // Workaround parses using the standard character set:
    utf16_len = 0;
    utf16_wc = NULL;
    if(mydict->is_utf8()){
      if(!format.compare("text")){
        parser = new TextParser(utf16_wc, utf16_len);
      } else if(!format.compare("latex")){
        parser = new LaTeXParser(utf16_wc, utf16_len);
      } else if(!format.compare("man")){
        parser = new ManParser(utf16_wc, utf16_len);
      } else if(!format.compare("xml")){
        parser = new XMLParser(utf16_wc, utf16_len);
      } else if(!format.compare("html")){
        parser = new HTMLParser(utf16_wc, utf16_len);
      } else {
        throw std::runtime_error("Unknown parse format");
      }
    } else {
      // 8bit encodings, e.g. latin1 or similar
      if(!format.compare("text")){
        parser = new TextParser(mydict->wc().c_str());
      } else if(!format.compare("latex")){
        parser = new LaTeXParser(mydict->wc().c_str());
      } else if(!format.compare("man")){
        parser = new ManParser(mydict->wc().c_str());
      } else {
        throw std::runtime_error("Unknown parse format");
      }
    }
  }

  ~hunspell_parser() {
    delete parser;
  }

  CharacterVector parse_text(String txt){
    CharacterVector words;
    txt.set_encoding(CE_UTF8);
    char * str = strdup(txt.get_cstring());
    parser->put_line(str);
    parser->set_url_checking(1);
    std::string token;
    while (parser->next_token(token)) {
      String x(token.c_str());
      x.set_encoding(CE_UTF8);
      words.push_back(x);
    }
    free(str);
    return words;
  }

  CharacterVector find(String txt, int i = 1){
    CharacterVector words;
    txt.set_encoding(CE_UTF8);
    char * str = mydict->string_from_r(txt);
    if(str == NULL){
      Rf_warningcall(R_NilValue, "Failed to convert line %d to %s encoding. Try spelling with a UTF8 dictionary.", i + 1, mydict->enc().c_str());
      return words;
    }
    parser->put_line(str);
    parser->set_url_checking(1);
    std::string token;
    while (parser->next_token(token)){
      if(!mydict->spell(token))
        words.push_back(mydict->string_to_r((char*) token.c_str()));
    }
    free(str);
    return words;
  }
};

// [[Rcpp::export]]
List R_hunspell_find(DictPtr ptr, StringVector text, std::string format, StringVector ignore){

  //init with affix and at least one dict
  hunspell_parser p(ptr.get(), format);

  //add ignore words
  ptr->add_words(ignore);

  int len = text.length();
  List out(len);
  for(int i = 0; i < len; i++)
    out[i] = p.find(text[i], i);
  return out;
}

// [[Rcpp::export]]
List R_hunspell_parse(DictPtr ptr, StringVector text, std::string format){

  //init with affix and at least one dict
  hunspell_parser p(ptr.get(), format);

  int len = text.length();
  List out(len);
  for(int i = 0; i < len; i++)
    out[i] = p.parse_text(text[i]);
  return out;
}
