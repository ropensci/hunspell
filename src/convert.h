#include <Rcpp.h>
#include <Riconv.h>

typedef iconv_t iconv_from_r_t;
typedef iconv_t iconv_to_r_t;

#define iconv_from_r(enc) (iconv_from_r_t) new_iconv("UTF-8", enc);
#define iconv_to_r(enc) (iconv_to_r_t) new_iconv(enc, "UTF-8");

iconv_t new_iconv(const char * from, const char * to);
char * string_from_r(Rcpp::String str, iconv_from_r_t cd, const char * enc);
Rcpp::String string_to_r(char * inbuf, iconv_to_r_t cd, const char * enc);
