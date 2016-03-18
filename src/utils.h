using namespace Rcpp;

class hunspell_dict {
  Hunspell * pMS_;
  char * enc_;
  iconv_t cd_from_;
  iconv_t cd_to_;

public:
  // Some strings are regular strings
  hunspell_dict(std::string affix, std::string dict){
    pMS_ = new Hunspell(affix.c_str(), dict.c_str());
    if(!pMS_)
      throw std::runtime_error(std::string("Failed to load file ") + dict.c_str());
    enc_ = pMS_->get_dic_encoding();
    if(!enc_)
      throw std::runtime_error("Failed to lookup encoding for dictionary");
    cd_from_ = iconv_from_r(enc_);
    cd_to_ = iconv_to_r(enc_);
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
    char * str = string_from_r(word, cd_from_);
    // Words that cannot be converted into the required encoding are by definition incorrect
    if(str == NULL)
      return false;
    bool res = pMS_->spell(str);
    free(str);
    return res;
  }

  void add_word(Rcpp::String word){
    char * str = string_from_r(word, cd_from_);
    if(str != NULL) {
      pMS_->add(str);
      free(str);
    }
  }

  CharacterVector suggest(Rcpp::String word){
    CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word, cd_from_);
    if(str != NULL){
      int ns = pMS_->suggest(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j], cd_to_));
      pMS_->free_list(&wlst, ns);
    }
    return res;
  }

  CharacterVector analyze(Rcpp::String word){
    CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word, cd_from_);
    if(str != NULL){
      int ns = pMS_->analyze(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j], cd_to_));
      pMS_->free_list(&wlst, ns);
    }
    return res;
  }

  CharacterVector stem(Rcpp::String word){
    CharacterVector res;
    char ** wlst;
    char * str = string_from_r(word, cd_from_);
    if(str != NULL){
      int ns = pMS_->stem(&wlst, str);
      free(str);
      for (int j = 0; j < ns; j++)
        res.push_back(string_to_r(wlst[j], cd_to_));
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
};
