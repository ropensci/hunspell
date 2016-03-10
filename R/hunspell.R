#' Hunspell Spell Checking
#'
#' Various tools for spell checking. The \code{\link{hunspell_check}} function
#' takes a vector of words and tests each individual word for correctness. The
#' \code{\link{hunspell_find}} function takes a character vector with text
#' (sentences) and returns only incorrect words. Finally \code{\link{hunspell_suggest}}
#' is used to suggest correct words for each (incorrect) input word.
#'
#' The functions \code{\link{hunspell_analyze}} and \code{\link{hunspell_stem}}
#' try to break down a word and return it's structure or stem word(s).
#'
#' Currently only US english dictionary is included with the package. Additional
#' dictrionaries can be downloaded from an OpenOffice
#' \href{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}{mirror}
#' or \href{http://archive.ubuntu.com/ubuntu/pool/main/libr/libreoffice-dictionaries/?C=S;O=D}{bundle}.
#'
#' @rdname hunspell
#' @aliases hunspell
#' @param words character vector with individual words to spellcheck
#' @param text character vector with arbitrary length text
#' @param ignore character vector with additional approved words dictionary
#' @param lang which dictionary to use. Currently only \code{en_US} is supported
#' @param delim string with characters used to deliminate words
#' @rdname hunspell
#' @importFrom Rcpp sourceCpp
#' @useDynLib hunspell
#' @export
#' @examples #check individual words
#' words <- c("beer", "wiskey", "wine")
#' correct <- hunspell_check(words)
#' print(correct)
#'
#' # find suggestions for incorrect words
#' hunspell_suggest(words[!correct])
#'
#' # find incorrect words in piece of text
#' bad <- hunspell_find("spell checkers are not neccessairy for langauge ninja's")
#' print(bad)
#' hunspell_suggest(bad)
hunspell_check <- function(words, ignore = character(), lang = "en_US"){
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  R_hunspell_check(get_affix(lang), get_dict(lang), words, ignore)
}

#' @rdname hunspell
#' @export
hunspell_find <- function(text, ignore = character(), delim = " .!?:;,.",  lang = "en_US"){
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  R_hunspell_find(get_affix(lang), get_dict(lang), text, ignore, delim)
}

#' @rdname hunspell
#' @export
hunspell_suggest <- function(words, lang = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_suggest(get_affix(lang), get_dict(lang), words)
}

#' @rdname hunspell
#' @export
hunspell_analyze <- function(words, lang = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_analyze(get_affix(lang), get_dict(lang), words)
}

#' @rdname hunspell
#' @export
hunspell_stem <- function(words, lang = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_stem(get_affix(lang), get_dict(lang), words)
}

get_affix <- function(lang){
  path <- system.file(paste0("dict/", lang, "/", lang, ".aff"), package = "hunspell")
  normalizePath(path, mustWork = TRUE)
}

get_dict <- function(lang){
  path <- system.file(paste0("dict/", lang, "/", lang, ".dic"), package = "hunspell")
  normalizePath(path, mustWork = TRUE)
}
