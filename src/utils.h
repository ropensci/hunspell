#include <hunspell.hxx>
#include <iconv.h>
#include <errno.h>
#include <Rcpp.h>

#ifdef _WIN32
#define ICONV_CONST_FIX (const char**)
#else
#define ICONV_CONST_FIX
#endif

class hunspell_dict {
  Hunspell * pMS_;
  char * enc_;
  iconv_t cd_from_;
  iconv_t cd_to_;

private:
  iconv_t new_iconv(const char * from, const char * to){
    iconv_t cd = iconv_open(to, from);
    if(cd == (iconv_t) -1){
      switch(errno){
        case EINVAL: throw std::runtime_error(std::string("Unsupported iconv conversion: ") + from + "to" + to);
        default: throw std::runtime_error("General error in iconv_open()");
      }
    }
    return cd;
  }

public:
  // Some strings are regular strings
  hunspell_dict(std::string affix, std::string dict){
    pMS_ = new Hunspell(affix.c_str(), dict.c_str());
    if(!pMS_)
      throw std::runtime_error(std::string("Failed to load file ") + dict.c_str());
    enc_ = pMS_->get_dic_encoding();
    if(!enc_)
      throw std::runtime_error("Failed to lookup encoding for dictionary");
    cd_from_ = new_iconv("UTF-8", enc_);
    cd_to_ = new_iconv(enc_, "UTF-8");
  }

  ~hunspell_dict() {
    try {
      iconv_close(cd_from_);
      iconv_close(cd_to_);
      delete pMS_;
    } catch (...) {}
  }

  unsigned short * get_wordchars_utf16(int *len){
    return pMS_->get_wordchars_utf16(len);
  }

  char * enc(){
    return enc_;
  }

  bool spell_char(char * word){
    return pMS_->spell(word);
  }

  bool spell(Rcpp::String word){
    char * str = string_from_r(word);
    // Words that cannot be converted into the required encoding are by definition incorrect
    if(str == NULL)
      return false;
    bool res = pMS_->spell(str);
    free(str);
    return res;
  }

  void add_word(Rcpp::String word){
    char * str = string_from_r(word);
    if(str != NULL) {
      pMS_->add(str);
      free(str);
    }
  }

  Rcpp::CharacterVector suggest(Rcpp::String word){
    Rcpp::CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word);
    if(str != NULL){
      int ns = pMS_->suggest(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j]));
      pMS_->free_list(&wlst, ns);
    }
    return res;
  }

  Rcpp::CharacterVector analyze(Rcpp::String word){
    Rcpp::CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word);
    if(str != NULL){
      int ns = pMS_->analyze(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j]));
      pMS_->free_list(&wlst, ns);
    }
    return res;
  }

  Rcpp::CharacterVector stem(Rcpp::String word){
    Rcpp::CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word);
    if(str != NULL){
      int ns = pMS_->stem(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j]));
      pMS_->free_list(&wlst, ns);
    }
    return res;
  }

  //adds ignore words to the dictionary
  void add_words(Rcpp::StringVector words){
    for(int i = 0; i < words.length(); i++){
      add_word(words[i]);
    }
  }

  iconv_t cd_from(){
    return cd_from_;
  }

  iconv_t cd_to(){
    return cd_to_;
  }

  char * wc(){
    return (char *) pMS_->get_wordchars();
  }

  char * string_from_r(Rcpp::String str){
    str.set_encoding(CE_UTF8);
    char * inbuf = (char *) str.get_cstring();
    size_t inlen = strlen(inbuf);
    size_t outlen = 4 * inlen + 1;
    char * output = (char *) malloc(outlen);
    char * cur = output;
    size_t success = iconv(cd_from_, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
    if(success == (size_t) -1){
      free(output);
      return NULL;
    }
    *cur = '\0';
    output = (char *) realloc(output, outlen + 1);
    return output;
  }

  Rcpp::String string_to_r(char * inbuf){
    if(inbuf == NULL)
      return NA_STRING;
    size_t inlen = strlen(inbuf);
    size_t outlen = 4 * inlen + 1;
    char * output = (char *) malloc(outlen);
    char * cur = output;
    size_t success = iconv(cd_to_, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
    if(success == (size_t) -1){
      free(output);
      return NA_STRING;
    }
    *cur = '\0';
    Rcpp::String res = Rcpp::String(output);
    res.set_encoding(CE_UTF8);
    free(output);
    return res;
  }
};
