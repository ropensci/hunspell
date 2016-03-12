#' Hunspell Spell Checking
#'
#' The \code{\link{hunspell_check}} function takes a vector of words and checks
#' each individual word for correctness. The \code{\link{hunspell_find}} function
#' takes a character vector with text (in plain, latex or man format) and returns
#' a list with incorrect words for each line. Finally \code{\link{hunspell_suggest}}
#' is used to suggest correct alternatives for each (incorrect) input word.
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
#' @param text character vector with arbitrary input text
#' @param ignore character vector with additional approved words for the dictionary
#' @param format input format; supported parsers are \code{text}, \code{latex} or \code{man}
#' @param lang dictionary language; currently only \code{en_US} is supported
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
#' print(bad[[1]])
#' hunspell_suggest(bad[[1]])
#'
#' \dontrun{
#' # check a latex document
#' download.file("http://arxiv.org/e-print/1406.4806v1", "1406.4806v1.tar.gz",  mode = "wb")
#' untar("1406.4806v1.tar.gz")
#' text <- readLines("content.tex", warn = FALSE)
#' words <- hunspell_find(text, format = "latex")
#' sort(unique(unlist(words)))
#' }
hunspell_check <- function(words, ignore = character(), lang = "en_US"){
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  R_hunspell_check(get_affix(lang), get_dict(lang), words, ignore)
}

#' @rdname hunspell
#' @export
hunspell_find <- function(text, ignore = character(), format = c("text", "man", "latex"),  lang = "en_US"){
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  format <- match.arg(format)
  R_hunspell_find(get_affix(lang), get_dict(lang), text, ignore, format)
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
