#include <hunspell.hxx>
#include <Rinternals.h>

extern "C" SEXP R_hunspell(SEXP affix, SEXP dict, SEXP words, SEXP ignore);

SEXP R_hunspell(SEXP affix, SEXP dict, SEXP words, SEXP ignore){

  //init with affix and at least one dict
  Hunspell * pMS = new Hunspell(CHAR(STRING_ELT(affix, 0)), CHAR(STRING_ELT(dict, 0)));

  //add additional dictionaries if more than one
  for(int i = 1; i < LENGTH(dict); i++){
    pMS->add_dic(CHAR(STRING_ELT(dict, i)));
  }

  //add ignore words
  for(int i = 0; i < LENGTH(ignore); i++){
    pMS->add(CHAR(STRING_ELT(ignore, i)));
  }

  //check words
  const char *buf;
  char ** wlst;
  for(int i = 0; i < LENGTH(words); i++){
    buf = CHAR(STRING_ELT(words, i));
    if(pMS->spell(buf)){
      Rprintf("OK: %s\n", buf);
    } else {
      Rprintf("WRONG: %s. SUGGESTIONS: ", buf);
      int ns = pMS->suggest(&wlst, buf);
      for (int i=0; i < ns; i++) {
        Rprintf("%s ", wlst[i]);
      }
      pMS->free_list(&wlst, ns);
      Rprintf("\n");
    }
  }

  delete pMS;
  return R_NilValue;
}
