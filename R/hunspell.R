#' Hunspell
#'
#' Run hunspell spell checker. Additional dictrionaries can be downloaded from
#' any OpenOffice \href{mirror}{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}.
#'
#' @rdname hunspell
#' @param words character vector of words to spellcheck
#' @param ignore additional approved words in dictionary
#' @rdname hunspell
#' @export
hunspell_en <- function(words, ignore = character()){
  affix <- system.file("dict/en_US/en_US.aff", package = "hunspell")
  dict <- system.file("dict/en_US/en_US.dic", package = "hunspell")
  hunspell(affix, dict, words, ignore)
}

#' @rdname hunspell
#' @useDynLib hunspell R_hunspell
hunspell <- function(affix, dict, words, ignore) {
  affix <- normalizePath(affix, mustWork = TRUE)
  dict <- normalizePath(dict, mustWork = TRUE)
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  .Call(R_hunspell, affix, dict, words, ignore)
  invisible()
}
