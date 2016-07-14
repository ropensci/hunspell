FROM rocker/r-devel-san

ENV UBSAN_OPTIONS print_stacktrace=1
ENV ASAN_OPTIONS malloc_context_size=10:fast_unwind_on_malloc=false
ENV SILENT "> /dev/null"

RUN apt-get update $SILENT \
	&& apt-get dist-upgrade $SILENT \
	&& apt-get install git pandoc pandoc-citeproc $SILENT \
	&& RD -e 'install.packages(c("Rcpp", "testthat"), quiet = TRUE)'

RUN git clone https://github.com/jeroenooms/hunspell \
	&& RD CMD build hunspell \
	&& RD CMD INSTALL hunspell_*.tar.gz --install-tests

RUN RDScript -e 'library(hunspell); testthat::test_dir("hunspell/tests/testthat")' || true

RUN RDScript -e 'library(hunspell); testthat::test_examples("hunspell/man")'|| true

RUN RD CMD check hunspell_*.tar.gz
