#include <errno.h>
#include "convert.h"

iconv_t new_iconv(const char * from, const char * to){
  iconv_t cd = iconv_open(to, from);
  if(cd == (iconv_t) -1){
    switch(errno){
      case EINVAL: std::runtime_error(std::string("Unsupported iconv conversion: ") + from + "to" + to);
      default: std::runtime_error("General error in iconv_open()");
    }
  }
  return cd;
}

char * string_from_r(Rcpp::String str, iconv_from_r_t cd){
  str.set_encoding(CE_UTF8);
  const char * inbuf = str.get_cstring();
  size_t inlen = strlen(inbuf);
  size_t outlen = inlen * 4;
  char output[outlen];
  char * outp = output;
  size_t success = iconv(cd, (char**) &inbuf, &inlen, &outp, &outlen);
  if(success == (size_t) -1){
    iconv_close(cd);
    switch(errno){
      case E2BIG: std::runtime_error("Iconv insufficient memory");
      case EILSEQ: std::runtime_error("An invalid multibyte sequence has been encountered in the input.");
      case EINVAL: std::runtime_error("An incomplete multibyte sequence has been encountered in the input.");
      default: std::runtime_error("General error in iconv()");
    }
  }
  outp[0] = '\0';
  char * res = (char *) malloc(outlen + 1);
  strcpy(res, output);
  return res;
}

Rcpp::String string_to_r(char * inbuf, iconv_to_r_t cd){
  size_t inlen = strlen(inbuf);
  size_t outlen = inlen * 4;
  char output[outlen];
  char * outp = output;
  size_t success = iconv(cd, &inbuf, &inlen, &outp, &outlen);
  if(success == (size_t) -1){
    iconv_close(cd);
    switch(errno){
      case E2BIG: std::runtime_error("Iconv insufficient memory");
      case EILSEQ: std::runtime_error("An invalid multibyte sequence has been encountered in the input.");
      case EINVAL: std::runtime_error("An incomplete multibyte sequence has been encountered in the input.");
      default: std::runtime_error("General error in iconv()");
    }
  }
  outp[0] = '\0';
  Rcpp::String res = Rcpp::String(output);
  res.set_encoding(CE_UTF8);
  return res;
}
