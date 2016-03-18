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
#' UTF8 dictionaries are available from \url{https://github.com/titoBouzout/Dictionaries}.
#'
#' @rdname hunspell
#' @aliases hunspell en_stats dicpath
#' @export en_stats dicpath
#' @param words character vector with individual words to spellcheck
#' @param text character vector with arbitrary input text
#' @param ignore character vector with additional approved words for the dictionary.
#' The default is \code{en_stats} which is included with base R.
#' @param format input format; supported parsers are \code{text}, \code{latex} or \code{man}
#' @param dict dictionary language, see details
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
#' # check a latex document
#' setwd(tempdir())
#' download.file("http://arxiv.org/e-print/1406.4806v1", "1406.4806v1.tar.gz",  mode = "wb")
#' untar("1406.4806v1.tar.gz")
#' text <- readLines("content.tex", warn = FALSE)
#' words <- hunspell_find(text, format = "latex")
#' sort(unique(unlist(words)))
hunspell_check <- function(words, ignore = en_stats, dict = "en_US"){
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  R_hunspell_check(get_affix(dict), get_dict(dict), words, ignore)
}

#' @rdname hunspell
#' @export
hunspell_find <- function(text, ignore = en_stats, format = c("text", "man", "latex"),  dict = "en_US"){
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  format <- match.arg(format)
  R_hunspell_find(get_affix(dict), get_dict(dict), text, ignore, format)
}

#' @rdname hunspell
#' @export
hunspell_suggest <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_suggest(get_affix(dict), get_dict(dict), words)
}

#' @rdname hunspell
#' @export
hunspell_analyze <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_analyze(get_affix(dict), get_dict(dict), words)
}

#' @rdname hunspell
#' @export
hunspell_stem <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  R_hunspell_stem(get_affix(dict), get_dict(dict), words)
}

#' @rdname hunspell
#' @export
hunspell_info <- function(dict = "en_US"){
  R_hunspell_info(get_affix(dict), get_dict(dict))
}

get_affix <- function(dict){
  files <- vapply(paste0(dict, ".aff"), find_in_dicpath, character(1))
  normalizePath(files, mustWork = TRUE)
}

get_dict <- function(dict){
  files <- vapply(paste0(dict, ".dic"), find_in_dicpath, character(1))
  normalizePath(files, mustWork = TRUE)
}

dicpath <- function(){
  c(
   Sys.getenv("DICPATH", getwd()),
   "~/Library/Spelling",
   "/usr/share/hunspell",
   "/usr/share/myspell",
   "/usr/share/myspell/dicts",
   "/Library/Spelling",
   system.file("dict", package = "hunspell"), # Bundled with the package
   "/usr/lib/rstudio/resources/dictionaries", #RS desktop, Linux
   "/usr/lib/rstudio-server/resources/dictionaries", #RS server, Linux
   "/Applications/RStudio.app/Contents/Resources/resources/dictionaries", #OSX
   "C:/Program Files/RStudio/resources/dictionaries" #Win
)}

find_in_dicpath <- function(name){
  paths <- file.path(dicpath(), name)
  found <- file.exists(paths)
  if(any(found)){
    return(paths[found][1])
  }
  stop("Dictionary file not found: ", name, call. = FALSE)
}

en_stats <- (function(){
  path <- file.path(R.home("share"), "dictionaries", "en_stats.rds")
  if(file.exists(path)){
    return(readRDS(path))
  } else {
    return(character(0))
  }
})()
