#include "utils.h"
#include "find.h"

using namespace Rcpp;
class Hunspell_dict_wrapper {
  private:
    hunspell_dict mydict;
    CharacterVector dict;

    enum E_FUN {suggest, analyze, stem, orignal};

    std::vector<E_FUN> process_mapping(Rcpp::StringVector functions){
      std::vector<E_FUN> out;
      for(int f = 0; f < functions.length(); f++){
        if(functions[f] == "suggest"){
          out.push_back(suggest);
        }else if(functions[f] == "analyze"){
          out.push_back(analyze);
        }else if(functions[f] == "stem"){
          out.push_back(stem);
        }else if(functions[f] == "orignal"){
          out.push_back(orignal);
        }
      }
      return out;
    }

    Rcpp::CharacterVector process_word(Rcpp::String word, std::vector<E_FUN> functions){
      Rcpp::CharacterVector out;
      for(int i = 0; i < functions.size(); i++){
        if(functions[i] == suggest){
          out = mydict.suggest(word);
        }else if(functions[i] == analyze){
          out = mydict.analyze(word);
        }else if(functions[i] == stem){
          out = mydict.stem(word);
        }else if(functions[i] == orignal){
          out = word;
        }
        if(!out.isNULL()){
          return out;
        }
      }
      return out;
    }

    List process_text(StringVector words, std::vector<E_FUN> mapping){
      List out;
      for(int i = 0; i < words.length(); i++)
        out.push_back(process_word(words[i],mapping));
      return out;

    }

  public:
    Hunspell_dict_wrapper(Rcpp::String affix_, CharacterVector dict_) : mydict(affix_, dict_) , dict(dict_) {}

    List R_hunspell_info(){
      return List::create(
        _["dict"] = dict,
        _["encoding"] = mydict.enc(),
        _["wordchars"] = mydict.r_wordchars()
      );
    }


    LogicalVector R_hunspell_check(StringVector words){
      LogicalVector out;
      for(int i = 0; i < words.length(); i++)
        out.push_back(mydict.spell(words[i]));
      return out;
    }
    List R_hunspell_check_list(List texts){
      List out;
      for(int i = 0; i < texts.length(); i++){
        out.push_back(R_hunspell_check(texts[i]));
      }
      return out;
    }


    List R_hunspell_suggest(StringVector words){
      List out;
      for(int i = 0; i < words.length(); i++)
        out.push_back(mydict.suggest(words[i]));
      return out;
    }
    List R_hunspell_suggest_list(List texts){
      List out;
      for(int i = 0; i < texts.length(); i++){
        out.push_back(R_hunspell_suggest(texts[i]));
      }
      return out;
    }


    List R_hunspell_analyze(StringVector words){
      List out;
      for(int i = 0; i < words.length(); i++){
        out.push_back(mydict.analyze(words[i]));
      }
      return out;
    }
    List R_hunspell_analyze_list(List texts){
      List out;
      for(int i = 0; i < texts.length(); i++){
        out.push_back(R_hunspell_analyze(texts[i]));
      }
      return out;
    }


    List R_hunspell_stem(StringVector words){
      List out;
      for(int i = 0; i < words.length(); i++)
        out.push_back(mydict.stem(words[i]));
      return out;
    }
    List R_hunspell_stem_list(List texts){
      List out;
      for(int i = 0; i < texts.length(); i++){
        out.push_back(R_hunspell_stem(texts[i]));
      }
      return out;
    }


    List R_hunspell_process(StringVector words, StringVector functions){
      std::vector<E_FUN> mapping = process_mapping(functions);
      return process_text(words,mapping);
    }
    List R_hunspell_process_list(List texts, StringVector functions){
      std::vector<E_FUN> mapping = process_mapping(functions);
      List out;
      for(int i = 0; i < texts.length(); i++){
        out.push_back(process_text(texts[i],mapping));
      }
      return out;
    }


    List R_hunspell_find(StringVector text, std::string format, StringVector ignore){

      hunspell_parser p(&mydict, format);

      //add ignore words
      mydict.add_words(ignore);

      List out;
      for(int i = 0; i < text.length(); i++)
        out.push_back(p.find(text[i], i));

      return out;
    }


    List R_hunspell_parse(StringVector text, std::string format){

      hunspell_parser p(&mydict, format);

      List out;
      for(int i = 0; i < text.length(); i++)
        out.push_back(p.parse_text(text[i]));

      return out;
    }

};

using namespace Rcpp;

RCPP_EXPOSED_CLASS(Hunspell_dict_wrapper)
  RCPP_MODULE(hunspell) {
    class_<Hunspell_dict_wrapper>("Hunspell_dict_wrapper")
    .constructor<Rcpp::String,CharacterVector>("creates dictionary")
    .method("R_hunspell_info", &Hunspell_dict_wrapper::R_hunspell_info, "R_hunspell_info")
    .method("R_hunspell_check", &Hunspell_dict_wrapper::R_hunspell_check, "R_hunspell_check")
    .method("R_hunspell_check_list", &Hunspell_dict_wrapper::R_hunspell_check_list, "R_hunspell_check_list")
    .method("R_hunspell_suggest", &Hunspell_dict_wrapper::R_hunspell_suggest, "R_hunspell_suggest")
    .method("R_hunspell_suggest_list", &Hunspell_dict_wrapper::R_hunspell_suggest_list, "R_hunspell_suggest_list")
    .method("R_hunspell_analyze", &Hunspell_dict_wrapper::R_hunspell_analyze, "R_hunspell_analyze")
    .method("R_hunspell_analyze_list", &Hunspell_dict_wrapper::R_hunspell_analyze_list, "R_hunspell_analyze_list")
    .method("R_hunspell_stem", &Hunspell_dict_wrapper::R_hunspell_stem, "R_hunspell_stem")
    .method("R_hunspell_stem_list", &Hunspell_dict_wrapper::R_hunspell_stem_list, "R_hunspell_stem_list")
    .method("R_hunspell_process", &Hunspell_dict_wrapper::R_hunspell_process, "R_hunspell_process")
    .method("R_hunspell_process_list", &Hunspell_dict_wrapper::R_hunspell_process_list, "R_hunspell_process_list")
    .method("R_hunspell_find", &Hunspell_dict_wrapper::R_hunspell_find, "R_hunspell_find")
    .method("R_hunspell_parse", &Hunspell_dict_wrapper::R_hunspell_parse, "R_hunspell_parse")
    ;
  }
