#' Hunspell Spell Checking and Morphological Analysis
#'
#' The \code{\link{hunspell}} function is a high-level wrapper for finding spelling
#' errors within a text document. It takes a character vector with text (\code{plain},
#' \code{latex} or \code{man} format), parses out the words and returns a list with
#' incorrect words for each line. It effectively combines of \code{\link{hunspell_parse}}
#' with \code{\link{hunspell_check}} in a single step. Other functions in the package
#' operate on individual words, see details.
#'
#' Hunspell uses a special dictionary format that defines which stems and affixes are
#' valid in a given language. The \code{\link{hunspell_analyze}} function shows how a
#' word breaks down into a valid stem plus affix. The \code{\link{hunspell_stem}}
#' function is similar but only returns valid stems for a given word. Stemming can be
#' used to summarize text (e.g in a wordcloud). The \code{\link{hunspell_check}} function
#' takes a vector of individual words and tests each one for correctness. Finally
#' \code{\link{hunspell_suggest}} is used to suggest correct alternatives for each
#' (incorrect) input word.
#'
#' Because spell checking is usually done on a document, the package includes some
#' parsers to extract words from various common formats. With \code{\link{hunspell_parse}}
#' we can parse plain-text, latex and man format. R also has a few built-in parsers
#' such as \code{\link[tools:RdTextFilter]{RdTextFilter}} and
#' \code{\link[tools:SweaveTeXFilter]{SweaveTeXFilter}}, see also
#' \code{\link[utils:aspell]{?aspell}}.
#'
#' The package searches for dictionaries in the working directory as well as in the
#' standard system locations. Additional search paths can be specified by setting
#' the \code{DICPATH} environment variable. A US English dictionary (\code{en_US}) is
#' included with the package; other dictionaries need to be installed by the system.
#' Most operating systems already include compatible dictionaries with names such as
#' \href{https://packages.debian.org/sid/hunspell-en-gb}{hunspell-en-gb} or
#' \href{https://packages.debian.org/sid/myspell-en-gb}{myspell-en-gb}.
#'
#' To manually install dictionaries, download the \code{.aff} and \code{.dic} file
#' from an OpenOffice \href{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}{mirror}
#' or \href{http://archive.ubuntu.com/ubuntu/pool/main/libr/libreoffice-dictionaries/?C=S;O=D}{bundle}
#' and copy them to \code{~/Library/Spelling} or a custom directory specified in \code{DICPATH}.
#' Alternatively you can pass the entire path to the \code{.dic} file as the \code{dict}
#' parameter.
#'
#' Note that \code{hunspell} uses \code{\link{iconv}} to convert input text to
#' the encoding used by the dictionary. This will fail if \code{text} contains characters
#' which are unsupported by that particular encoding. For this reason UTF-8 dictionaries
#' are preferable over legacy 8bit dictionaries Several UTF8 dictionaries are
#' available from \href{https://github.com/titoBouzout/Dictionaries}{Github}.
#'
#' @rdname hunspell
#' @aliases hunspell hunspell_find en_stats dicpath
#' @export en_stats dicpath
#' @param words character vector with individual words to spell check
#' @param text character vector with arbitrary input text
#' @param ignore character vector with additional approved words added to the dictionary
#' @param format input format; supported parsers are \code{text}, \code{latex} or \code{man}
#' @param dict dictionary language, see details
#' @rdname hunspell
#' @importFrom Rcpp sourceCpp
#' @useDynLib hunspell
#' @export hunspell hunspell_find
#' @examples # Check individual words
#' words <- c("beer", "wiskey", "wine")
#' correct <- hunspell_check(words)
#' print(correct)
#'
#' # Find suggestions for incorrect words
#' hunspell_suggest(words[!correct])
#'
#' # Extract incorrect from a piece of text
#' bad <- hunspell("spell checkers are not neccessairy for langauge ninja's")
#' print(bad[[1]])
#' hunspell_suggest(bad[[1]])
#'
#' # Stemming
#' words <- c("love", "loving", "lovingly", "loved", "lover", "lovely", "love")
#' hunspell_stem(words)
#' hunspell_analyze(words)
#'
#' # Check an entire latex document
#' setwd(tempdir())
#' download.file("http://arxiv.org/e-print/1406.4806v1", "1406.4806v1.tar.gz",  mode = "wb")
#' untar("1406.4806v1.tar.gz")
#' text <- readLines("content.tex", warn = FALSE)
#' bad_words <- hunspell(text, format = "latex")
#' sort(unique(unlist(bad_words)))
#'
#' # Summarize text by stems (e.g. for wordcloud)
#' allwords <- hunspell_parse(text, format = "latex")
#' stems <- unlist(hunspell_stem(unlist(allwords)))
#' words <- head(sort(table(stems), decreasing = TRUE), 200)
hunspell <- function(text, format = c("text", "man", "latex"), dict = "en_US", ignore = en_stats){
  stopifnot(is.character(text))
  stopifnot(is.character(ignore))
  format <- match.arg(format)
  dicpath <- get_dict(dict)
  R_hunspell_find(get_affix(dicpath), dicpath, text, format, ignore)
}

#for backward compatiblity
hunspell_find <- hunspell

#' @rdname hunspell
#' @export
hunspell_parse <- function(text, format = c("text", "man", "latex"), dict = "en_US"){
  stopifnot(is.character(text))
  format <- match.arg(format)
  dicpath <- get_dict(dict)
  R_hunspell_parse(get_affix(dicpath), dicpath, text, format)
}

#' @rdname hunspell
#' @export
hunspell_check <- function(words, dict = "en_US"){
  stopifnot(is.character(words))
  dicpath <- get_dict(dict)
  R_hunspell_check(get_affix(dicpath), dicpath, words)
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
  normalizePath(find_in_dicpath(paste0(dict, ".dic")), mustWork = TRUE)
}

dicpath <- function(){
  c(
   Sys.getenv("DICPATH", getwd()),
   system.file("dict", package = "hunspell"), # Bundled with the R package
   normalizePath("~/Library/Spelling", mustWork = FALSE),
   "/usr/share/hunspell",
   "/usr/share/myspell",
   "/usr/share/myspell/dicts",
   "/Library/Spelling",
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
