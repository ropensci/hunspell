FROM rocker/r-devel-san

ENV UBSAN_OPTIONS print_stacktrace=1
ENV ASAN_OPTIONS malloc_context_size=10:fast_unwind_on_malloc=false

RUN apt-get -qq update > /dev/null \
	&& apt-get -qq dist-upgrade > /dev/null \
	&& apt-get -qq install git pandoc pandoc-citeproc > /dev/null \
	&& RDscript -e 'install.packages(c("Rcpp", "testthat"), quiet = TRUE)'

RUN git clone https://github.com/jeroen/hunspell \
	&& RD CMD build hunspell \
	&& RD CMD INSTALL hunspell_*.tar.gz --install-tests

RUN RDscript -e 'sessionInfo()'

RUN RDscript -e 'library(hunspell); testthat::test_dir("hunspell/tests/testthat")' || true

RUN RDscript -e 'library(hunspell); testthat::test_examples("hunspell/man")'|| true

RUN RD CMD check hunspell_*.tar.gz
