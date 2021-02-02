# rOpenSci: The *hunspell* package

> High-Performance Stemmer, Tokenizer, and Spell Checker for R

[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](http://www.repostatus.org/badges/latest/active.svg)](http://www.repostatus.org/#active)
[![Build Status](https://travis-ci.org/ropensci/hunspell.svg?branch=master)](https://travis-ci.org/ropensci/hunspell)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/ropensci/hunspell?branch=master&svg=true)](https://ci.appveyor.com/project/jeroen/hunspell)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/hunspell)](https://cran.r-project.org/package=hunspell)
[![CRAN RStudio mirror downloads](http://cranlogs.r-pkg.org/badges/hunspell)](https://cran.r-project.org/package=hunspell)
[![Github Stars](https://img.shields.io/github/stars/ropensci/hunspell.svg?style=social&label=Github)](https://github.com/ropensci/hunspell)

Low level spell checker and morphological analyzer based on the 
famous hunspell library <https://hunspell.github.io>. The package can analyze
or check individual words as well as tokenize text, latex, html or xml documents.
For a more user-friendly interface use the 'spelling' package which builds on
this package with utilities to automate checking of files, documentation and 
vignettes in all common formats.

## Installation

This package includes a bundled version of libhunspell and no longer
depends on external system libraries:

```r
install.packages("hunspell")
```


## Documentation

About the R package:
 - Blog post: [Hunspell: Spell Checker and Text Parser for R](https://www.opencpu.org/posts/hunspell-release/)
 - Blog post: [Stemming and Spell Checking in R](https://www.opencpu.org/posts/hunspell-1-2/)

## Hello World

```r
# Check individual words
words <- c("beer", "wiskey", "wine")
correct <- hunspell_check(words)
print(correct)

# Find suggestions for incorrect words
hunspell_suggest(words[!correct])

# Extract incorrect from a piece of text
bad <- hunspell("spell checkers are not neccessairy for langauge ninja's")
print(bad[[1]])
hunspell_suggest(bad[[1]])

# Stemming
words <- c("love", "loving", "lovingly", "loved", "lover", "lovely", "love")
hunspell_stem(words)
hunspell_analyze(words)
```

The [spelling](https://docs.ropensci.org/spelling) package uses this package to spell R package documentation:

```r
# Spell check a package
library(spelling)
spell_check_package("~/mypackage")
```

[![](http://ropensci.org/public_images/github_footer.png)](http://ropensci.org)
