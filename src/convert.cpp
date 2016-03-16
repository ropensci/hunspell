#include <errno.h>
#include "convert.h"

#ifdef _WIN32
#define ICONV_CONST_FIX (const char**)
#else
#define ICONV_CONST_FIX
#endif

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

char * string_from_r(Rcpp::String str, iconv_t cd){
  str.set_encoding(CE_UTF8);
  char * inbuf = (char *) str.get_cstring();
  size_t inlen = strlen(inbuf);
  size_t outlen = 4 * inlen + 1;
  char * output = (char *) malloc(outlen);
  char * cur = output;
  size_t success = iconv(cd, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
  if(success == (size_t) -1){
    free(output);
    return NULL;
  }
  *cur = '\0';
  output = (char *) realloc(output, outlen + 1);
  return output;
}

Rcpp::String string_to_r(char * inbuf, iconv_t cd){
  size_t inlen = strlen(inbuf);
  size_t outlen = 4 * inlen + 1;
  char * output = (char *) malloc(outlen);
  char * cur = output;
  size_t success = iconv(cd, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
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
