#include <Rcpp.h>
#include <iconv.h>
#include <errno.h>

char * r_to_string(Rcpp::String str, char * enc){
  str.set_encoding(CE_UTF8);
  iconv_t cd = iconv_open(enc, "UTF-8");
  if(cd == (iconv_t) -1){
    switch(errno){
    case EINVAL: std::runtime_error(std::string("Iconv does not support UTF8 conversion for ") + enc);
    default: std::runtime_error("General error in iconv_open()");
    }
  }
  const char * inbuf = str.get_cstring();
  size_t inlen = strlen(inbuf);
  size_t outlen = inlen * 4;
  char output[outlen];
  char * outp = output;
  size_t success = iconv(cd, (char**) &inbuf, &inlen, &outp, &outlen);
  iconv_close(cd);
  if(success == (size_t) -1){
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

Rcpp::String string_to_r(char * inbuf, char * enc){
  iconv_t cd = iconv_open("UTF-8", enc);
  if(cd == (iconv_t) -1){
    switch(errno){
    case EINVAL: std::runtime_error(std::string("Iconv does not support UTF8 conversion for ") + enc);
    default: std::runtime_error("General error in iconv_open()");
    }
  }
  size_t inlen = strlen(inbuf);
  size_t outlen = inlen * 4;
  char output[outlen];
  char * outp = output;
  size_t success = iconv(cd, &inbuf, &inlen, &outp, &outlen);
  iconv_close(cd);
  if(success == (size_t) -1){
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
