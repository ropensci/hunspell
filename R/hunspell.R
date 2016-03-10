#' Hunspell
#'
#' Run hunspell spell checker. The \code{hunspell_check} functions return
#' a logical vector of equal length to the input indicating whether each
#' word was correct. The \code{hunspell_suggest} functions return a list
#' with character vectors of similar words as the input.
#'
#' Currently only US english dictionary is included with the package.
#' Additional dictrionaries can be downloaded from e.g. OpenOffice
#' \href{mirrors}{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}.
#'
#' @rdname hunspell
#' @param words character vector of words to spellcheck
#' @param ignore additional approved words in dictionary
#' @rdname hunspell
#' @importFrom Rcpp sourceCpp
#' @useDynLib hunspell
#' @export
#' @examples words <- c("beer", "wiskey", "wine")
#' correct <- hunspell_check_en(words)
#' print(correct)
#'
#' # find suggestions for incorrect words
#' hunspell_suggest_en(words[!correct])
hunspell_check_en <- function(words, ignore = character()){
  affix <- system.file("dict/en_US/en_US.aff", package = "hunspell")
  dict <- system.file("dict/en_US/en_US.dic", package = "hunspell")
  hunspell_check(affix, dict, words, ignore)
}

#' @rdname hunspell
#' @export
hunspell_find_en <- function(text, ignore = character()){
  affix <- system.file("dict/en_US/en_US.aff", package = "hunspell")
  dict <- system.file("dict/en_US/en_US.dic", package = "hunspell")
  hunspell_find(affix, dict, text, ignore)
}

#' @rdname hunspell
#' @export
hunspell_suggest_en <- function(words){
  affix <- system.file("dict/en_US/en_US.aff", package = "hunspell")
  dict <- system.file("dict/en_US/en_US.dic", package = "hunspell")
  hunspell_suggest(affix, dict, words)
}

hunspell_check <- function(affix, dict, words, ignore) {
  affix <- normalizePath(affix, mustWork = TRUE)
  dict <- normalizePath(dict, mustWork = TRUE)
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  R_hunspell_check(affix, dict, words, ignore)
}

hunspell_find <- function(affix, dict, text, ignore) {
  affix <- normalizePath(affix, mustWork = TRUE)
  dict <- normalizePath(dict, mustWork = TRUE)
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  R_hunspell_find(affix, dict, text, ignore)
}

hunspell_suggest <- function(affix, dict, words) {
  affix <- normalizePath(affix, mustWork = TRUE)
  dict <- normalizePath(dict, mustWork = TRUE)
  stopifnot(is.character(words))
  R_hunspell_suggest(affix, dict, words)
}
