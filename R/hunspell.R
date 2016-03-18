#' Hunspell Spell Checking
#'
#' The \code{\link{hunspell_check}} function takes a vector of individualwords and tests
#' each one for correctness. The \code{\link{hunspell_find}} function takes a character
#' vector with text (plain, latex or man format), parses out the words and returns a
#' list with incorrect words for each line. Finally \code{\link{hunspell_suggest}}
#' is used to suggest correct alternatives for each (incorrect) input word.
#'
#' The \code{\link{hunspell_analyze}} function shows how a word breaks down into a
#' valid stem plus affix. The \code{\link{hunspell_stem}} function only returns valid
#' stems for a given word. Hunspell uses a special dictionary format that defines
#' which stems and affixes are valid in a given language. Among other things, stemming
#' can be used to summarize text (e.g wordcloud).
#'
#' The package searches for dictionaries in the working directory as well as in the
#' standard system dictionary locations. Additional search paths can be added by setting
#' the \code{DICPATH} environment variable. A US english dictionary is included with
#' the package; other dictionaries need to be installed by the system. Most operating
#' systems already include standard dictionaries with names such as
#' \href{https://packages.debian.org/sid/hunspell-en-gbl}{hunspell-en-gb} or
#' \href{https://packages.debian.org/sid/myspell-en-gb}{myspell-en-gb}.
#'
#' To manually install dictionaries, download the \code{.aff} and \code{.dic} file
#' from an OpenOffice \href{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}{mirror}
#' or \href{http://archive.ubuntu.com/ubuntu/pool/main/libr/libreoffice-dictionaries/?C=S;O=D}{bundle}
#' and copy them to \code{~/Library/Spelling} or a custom directory specified in \code{DICPATH}.
#' Alternatively you can pass the entire path to the \code{.aff} and \code{.dic} file
#' as the \code{dict} paramter.
#'
#' Note that \code{hunspell_find} uses iconv to convert input text to the encoding
#' used by the dictionary. This will fail if \code{text} contains characters which are
#' unsupported by that particular encoding. For this reason it is safer to use UTF8
#' dictionaries which can represent all unicode characters. Several UTF8 dictionaries are
#' available from \href{https://github.com/titoBouzout/Dictionaries}{Github}.
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
#' @examples # Check individual words
#' words <- c("beer", "wiskey", "wine")
#' correct <- hunspell_check(words)
#' print(correct)
#'
#' # Find suggestions for incorrect words
#' hunspell_suggest(words[!correct])
#'
#' # Extract incorrect from a piece of text
#' bad <- hunspell_find("spell checkers are not neccessairy for langauge ninja's")
#' print(bad[[1]])
#' hunspell_suggest(bad[[1]])
#'
#' # Check an entire latex document
#' setwd(tempdir())
#' download.file("http://arxiv.org/e-print/1406.4806v1", "1406.4806v1.tar.gz",  mode = "wb")
#' untar("1406.4806v1.tar.gz")
#' text <- readLines("content.tex", warn = FALSE)
#' words <- hunspell_find(text, format = "latex")
#' sort(unique(unlist(words)))
hunspell_check <- function(words, ignore = en_stats, dict = "en_US"){
  stopifnot(is.character(words))
  stopifnot(is.character(ignore))
  dicpath <- get_dict(dict)
  R_hunspell_check(get_affix(dicpath), dicpath, words, ignore)
}

#' @rdname hunspell
#' @export
hunspell_find <- function(text, ignore = en_stats, format = c("text", "man", "latex"),  dict = "en_US"){
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  format <- match.arg(format)
  dicpath <- get_dict(dict)
  R_hunspell_find(get_affix(dicpath), dicpath, text, ignore, format)
}

#' @rdname hunspell
#' @export
hunspell_suggest <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  dicpath <- get_dict(dict)
  R_hunspell_suggest(get_affix(dicpath), dicpath, words)
}

#' @rdname hunspell
#' @export
hunspell_analyze <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  dicpath <- get_dict(dict)
  R_hunspell_analyze(get_affix(dicpath), dicpath, words)
}

#' @rdname hunspell
#' @export
hunspell_stem <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  dicpath <- get_dict(dict)
  R_hunspell_stem(get_affix(dicpath), dicpath, words)
}

#' @rdname hunspell
#' @export
hunspell_info <- function(dict = "en_US"){
  dicpath <- get_dict(dict)
  R_hunspell_info(get_affix(dicpath), dicpath)
}

get_affix <- function(dicpath){
  normalizePath(sub("\\.dic$", ".aff", dicpath), mustWork = TRUE)
}

get_dict <- function(dict){
  dict <- sub("\\.(dic|aff)$", "", dict)
  files <- vapply(paste0(dict, ".dic"), find_in_dicpath, character(1))
  normalizePath(files, mustWork = TRUE)
}

dicpath <- function(){
  c(
   Sys.getenv("DICPATH", getwd()),
   normalizePath("~/Library/Spelling", mustWork = FALSE),
   "/usr/share/hunspell",
   "/usr/share/myspell",
   "/usr/share/myspell/dicts",
   "/Library/Spelling",
   system.file("dict", package = "hunspell"), # Bundled with the R package
   file.path(dirname(Sys.getenv("RMARKDOWN_MATHJAX_PATH")), "dictionaries") #Rstudio
  )
}

find_in_dicpath <- function(name){
  paths <- c(normalizePath(name, mustWork = FALSE), file.path(dicpath(), name))
  found <- file.exists(paths)
  if(any(found))
    return(paths[found][1])
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
