#include <Rcpp.h>
#include <iconv.h>

#define iconv_from_r(enc) new_iconv("UTF-8", enc);
#define iconv_to_r(enc) new_iconv(enc, "UTF-8");

iconv_t new_iconv(const char * from, const char * to);
char * string_from_r(Rcpp::String str, iconv_t cd);
Rcpp::String string_to_r(char * inbuf, iconv_t cd);
