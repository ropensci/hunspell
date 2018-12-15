#' Hunspell Spell Checking and Morphological Analysis
#'
#' The \code{\link{hunspell}} function is a high-level wrapper for finding spelling
#' errors within a text document. It takes a character vector with text (\code{plain},
#' \code{latex}, \code{man}, \code{html} or \code{xml} format), parses out the words
#' and returns a list with incorrect words for each line. It effectively combines
#' \code{\link{hunspell_parse}} with \code{\link{hunspell_check}} in a single step.
#' Other functions in the package operate on individual words, see details.
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
#' standard system locations. \code{\link{list_dictionaries}} provides a list of all
#' dictionaries it can find. Additional search paths can be specified by setting
#' the \code{DICPATH} environment variable. A US English dictionary (\code{en_US}) is
#' included with the package; other dictionaries need to be installed by the system.
#' Most operating systems already include compatible dictionaries with names such as
#' \href{https://packages.debian.org/sid/hunspell-en-gb}{hunspell-en-gb} or
#' \href{https://packages.debian.org/sid/myspell-en-gb}{myspell-en-gb}.
#'
#' To manually install dictionaries, copy the corresponding \code{.aff} and \code{.dic}
#' file to \code{~/Library/Spelling} or a custom directory specified in \code{DICPATH}.
#' Alternatively you can pass the entire path to the \code{.dic} file as the \code{dict}
#' parameter. Some popular sources of dictionaries are
#' \href{http://wordlist.aspell.net/dicts/}{SCOWL},
#' \href{http://ftp.snt.utwente.nl/pub/software/openoffice/contrib/dictionaries/}{OpenOffice},
#' \href{http://archive.ubuntu.com/ubuntu/pool/main/libr/libreoffice-dictionaries/?C=S;O=D}{debian},
#' \href{https://github.com/titoBouzout/Dictionaries}{github/titoBouzout} or
#' \href{https://github.com/wooorm/dictionaries}{github/wooorm}.
#'
#' Note that \code{hunspell} uses \code{\link{iconv}} to convert input text to
#' the encoding used by the dictionary. This will fail if \code{text} contains characters
#' which are unsupported by that particular encoding. For this reason UTF-8 dictionaries
#' are preferable over legacy 8-bit dictionaries.
#'
#' @rdname hunspell
#' @aliases hunspell hunspell_find en_stats dicpath
#' @export en_stats dicpath
#' @param dict a dictionary object or string which can be passed to \code{\link{dictionary}}.
#' @param words character vector with individual words to spell check
#' @param text character vector with arbitrary input text
#' @param ignore character vector with additional approved words added to the dictionary
#' @param format input format; supported parsers are \code{text}, \code{latex}, \code{man},
#' \code{xml} and \code{html}.
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
#' tmpfile <- file.path(tempdir(), "1406.4806v1.tar.gz")
#' download.file("https://arxiv.org/e-print/1406.4806v1", tmpfile,  mode = "wb")
#' untar(tmpfile, exdir = tempdir())
#' text <- readLines(file.path(tempdir(), "content.tex"), warn = FALSE)
#' bad_words <- hunspell(text, format = "latex")
#' sort(unique(unlist(bad_words)))
#'
#' # Summarize text by stems (e.g. for wordcloud)
#' allwords <- hunspell_parse(text, format = "latex")
#' stems <- unlist(hunspell_stem(unlist(allwords)))
#' words <- head(sort(table(stems), decreasing = TRUE), 200)
hunspell <- function(text, format = c("text", "man", "latex", "html", "xml"),
                     dict = dictionary("en_US"), ignore = en_stats){
  stopifnot(is.character(text))
  ignore <- as.character(ignore)
  format <- match.arg(format)
  dictionary <- dictionary(dict, add_words = ignore)
  R_hunspell_find(dictionary, text, format)
}

#for backward compatiblity
hunspell_find <- hunspell

#' @rdname hunspell
#' @export
hunspell_parse <- function(text, format = c("text", "man", "latex", "html", "xml"),
                           dict = dictionary("en_US")){
  stopifnot(is.character(text))
  format <- match.arg(format)
  dictionary <- dictionary(dict)
  R_hunspell_parse(dictionary, text, format)
}

#' @rdname hunspell
#' @export
hunspell_check <- function(words, dict = dictionary("en_US")){
  stopifnot(is.character(words))
  dictionary <- dictionary(dict)
  R_hunspell_check(dictionary, words)
}

#' @rdname hunspell
#' @export
hunspell_suggest <- function(words, dict = dictionary("en_US")){
  stopifnot(is.character(words))
  dictionary <- dictionary(dict)
  R_hunspell_suggest(dictionary, words)
}

#' @rdname hunspell
#' @export
hunspell_analyze <- function(words, dict = dictionary("en_US")){
  stopifnot(is.character(words))
  dictionary <- dictionary(dict)
  R_hunspell_analyze(dictionary, words)
}

#' @rdname hunspell
#' @export
hunspell_stem <- function(words, dict = dictionary("en_US")){
  stopifnot(is.character(words))
  dictionary <- dictionary(dict)
  R_hunspell_stem(dictionary, words)
}

#' @rdname hunspell
#' @export
hunspell_info <- function(dict = dictionary("en_US")){
  dictionary <- dictionary(dict)
  info <- R_hunspell_info(dictionary)
  if(length(info$wordchars)){
    wc_enc <- ifelse(info$encoding == "UTF-8", "UTF-16LE", info$encoding)
    wc <- iconv(list(info$wordchars), wc_enc, "UTF-8")
    Encoding(wc) <- "UTF-8"
    info$wordchars <- wc
  } else {
    info$wordchars <- NA_character_
  }
  info
}

dictionary_load <- function(lang, affix, add_words, cache){
  dict <- get_dict(lang)
  affix <- if(length(affix)){
    normalizePath(affix, mustWork = TRUE)
  } else {
    get_affix(dict)
  }
  # Workaround for https://github.com/hunspell/hunspell/issues/616
  add_words <- chartr("\u2019", "'", as.character(add_words))
  if(!isTRUE(cache))
    return(dictionary_new(dict, affix, add_words))
  key <- digest::digest(list(dict, affix, add_words))
  if(!exists(key, store, inherits = FALSE))
    store[[key]] <- dictionary_new(dict, affix, add_words)
  return(store[[key]])
}

dictionary_new <- function(dict, affix, add_words){
  out <- R_hunspell_dict(affix, dict, add_words)
  structure(out, class = "hunspell_dictionary")
}

get_affix <- function(dicpath){
  normalizePath(sub("\\.dic$", ".aff", dicpath), mustWork = TRUE)
}

get_dict <- function(dict){
  if(!file.exists(dict)){
    dict <- find_in_dicpath(paste0(sub("\\.(dic|aff)$", "", dict), ".dic"))
  }
  normalizePath(dict, mustWork = TRUE)
}

rstudio_dicpaths <- function(){
  paths <- file.path(dirname(Sys.getenv("RMARKDOWN_MATHJAX_PATH")), "dictionaries")
  subdirs <- c('languages-system', 'languages-user')
  if(.Platform$OS.type == 'windows'){
    paths <- c(paths, file.path(Sys.getenv('localappdata'), 'RStudio-Desktop', 'dictionaries', subdirs))
  } else {
    if(file.exists('~/.rstudio-desktop')){
      paths <- c(paths, file.path('~/.rstudio-desktop', 'dictionaries', subdirs))
    }
    if(file.exists('~/.rstudio')){
      paths <- c(paths, file.path('~/.rstudio', 'dictionaries', subdirs))
    }
  }
  return(paths)
}

dicpath <- function(){
  c(
   Sys.getenv("DICPATH", getwd()),
   system.file("dict", package = "hunspell"), # Bundled with the R package
   normalizePath("~/Library/Spelling", mustWork = FALSE),
   "/usr/local/share/hunspell",
   "/usr/local/share/myspell",
   "/usr/local/share/myspell/dicts",
   "/usr/share/hunspell",
   "/usr/share/myspell",
   "/usr/share/myspell/dicts",
   "/Library/Spelling",
   rstudio_dicpaths()
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

#' @export
print.hunspell_dictionary <- function(x, ...){
  info <- hunspell_info(x)
  cat("<hunspell dictionary>\n")
  cat(" affix:", info$affix, "\n")
  cat(" dictionary:", info$dict, "\n")
  cat(" encoding:", info$encoding, "\n")
  cat(" wordchars:", info$wordchars, "\n")
  cat(" added:", length(info$added), "custom words\n")
  invisible()
}

#' @export
#' @rdname hunspell
#' @param lang dictionary file or language, see details
#' @param affix file path to corresponding affix file. If \code{NULL} it is
#' is assumed to be the same path as \code{dict} with extension \code{.aff}.
#' @param cache speed up loading of dictionaries by caching
#' @param add_words a character vector of additional words to add to the dictionary
dictionary <- function(lang = "en_US", affix = NULL, add_words = NULL, cache = TRUE){
  add_words <- sort(unique(as.character(add_words)))
  if(inherits(lang, "hunspell_dictionary")){
    if(!length(add_words))
      return(lang)
    info <- R_hunspell_info(lang)
    lang <- info$dict
    affix <- info$affix
    add_words <- sort(unique(c(info$added, add_words)))
  }
  dictionary_load(lang = lang, affix = affix, add_words = add_words, cache = cache)
}

store <- new.env()

#' @export
#' @rdname hunspell
list_dictionaries <- function() {
  dic_file <- list.files(dicpath(), pattern = "\\.dic$")
  aff_file <- list.files(dicpath(), pattern = "\\.aff$")
  dic_name <- substr(dic_file, 1 , nchar(dic_file) - 4)
  aff_name <- substr(aff_file, 1 , nchar(aff_file) - 4)
  return(intersect(dic_name, aff_name))
}
