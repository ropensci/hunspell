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

char * string_convert(char * inbuf, iconv_t cd){
  size_t inlen = strlen(inbuf);
  size_t outlen = 4 * inlen + 1;
  char output[outlen];
  char * cur = output;
  size_t success = iconv(cd, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
  if(success == (size_t) -1)
    return NULL;
  *cur = '\0';
  char * res = (char *) malloc(outlen + 1);
  strcpy(res, output);
  return res;
}

char * string_from_r(Rcpp::String str, iconv_t cd){
  str.set_encoding(CE_UTF8);
  return string_convert((char *) str.get_cstring(), cd);
}

Rcpp::String string_to_r(char * inbuf, iconv_t cd){
  char * output = string_convert(inbuf, cd);
  if(output == NULL)
    return NA_STRING;
  Rcpp::String res = Rcpp::String(output);
  free(output);
  res.set_encoding(CE_UTF8);
  return res;
}

unsigned short * string_to_utf16(char * inbuf, char * enc, size_t * size){
  iconv_t cd = new_iconv(enc, "UTF-16LE");
  size_t inlen = strlen(inbuf);
  size_t outlen = 4 * inlen + 1;
  char output[outlen];
  char * cur = output;
  size_t success = iconv(cd, ICONV_CONST_FIX &inbuf, &inlen, &cur, &outlen);
  if(success == (size_t) -1)
    return NULL;
  *size = outlen - 1;
  unsigned short * res = (unsigned short *) malloc(outlen - 1);
  memcpy(res, output, outlen - 1);
  return res;
}
