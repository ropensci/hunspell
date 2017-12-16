#include "dictionary.h"

inline void dict_finalizer(hunspell_dict *dict) {
  delete dict;
}

typedef Rcpp::XPtr<hunspell_dict, Rcpp::PreserveStorage, dict_finalizer> DictPtr;
