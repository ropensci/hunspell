#' Hunspell
#'
#' Run hunspell spell checker. Additional dictrionaries can be downloaded from
#' any OpenOffice \href{mirror}{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}.
#'
#' @rdname hunspell
#' @param words character vector of words to spellcheck
#' @param ignore additional approved words in dictionary
#' @rdname hunspell
#' @importFrom Rcpp sourceCpp
#' @useDynLib hunspell
#' @export
hunspell_check_en <- function(words, ignore = character()){
  affix <- system.file("dict/en_US/en_US.aff", package = "hunspell")
  dict <- system.file("dict/en_US/en_US.dic", package = "hunspell")
  hunspell_check(affix, dict, words, ignore)
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

hunspell_suggest <- function(affix, dict, words) {
  affix <- normalizePath(affix, mustWork = TRUE)
  dict <- normalizePath(dict, mustWork = TRUE)
  stopifnot(is.character(words))
  R_hunspell_suggest(affix, dict, words)
}
