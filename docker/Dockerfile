FROM rocker/r-devel-san

ENV UBSAN_OPTIONS print_stacktrace=1
ENV ASAN_OPTIONS malloc_context_size=10:fast_unwind_on_malloc=false

RUN apt-get -qq update \
	&& apt-get -qq dist-upgrade -y \
	&& RD -e 'install.packages(c("Rcpp", "testthat"), quiet = TRUE)'

RUN git clone https://github.com/jeroenooms/hunspell \
	&& RD CMD build hunspell \
	&& RD CMD INSTALL hunspell_*.tar.gz --install-tests

RUN RD -e 'testthat::test_package("hunspell"); q("no");' || true

RUN RD CMD check hunspell_*.tar.gz
