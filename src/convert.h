#include <Rcpp.h>

char * r_to_string(Rcpp::String str, char * enc);
Rcpp::String string_to_r(char * inbuf, char * enc);
