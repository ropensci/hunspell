# hunspell

##### *Morphological Analysis and Spell Checker for R*

[![Build Status](https://travis-ci.org/ropensci/hunspell.svg?branch=master)](https://travis-ci.org/ropensci/hunspell)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/ropensci/hunspell?branch=master&svg=true)](https://ci.appveyor.com/project/jeroenooms/hunspell)
[![Coverage Status](https://codecov.io/github/ropensci/hunspell/coverage.svg?branch=master)](https://codecov.io/github/ropensci/hunspell?branch=master)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/hunspell)](http://cran.r-project.org/package=hunspell)
[![CRAN RStudio mirror downloads](http://cranlogs.r-pkg.org/badges/hunspell)](http://cran.r-project.org/web/packages/hunspell/index.html)
[![Github Stars](https://img.shields.io/github/stars/ropensci/hunspell.svg?style=social&label=Github)](https://github.com/ropensci/hunspell)

> A spell checker and morphological analyzer library designed for
  languages with rich morphology and complex word compounding or character
  encoding. The package can check and analyze individual words as well as
  search for incorrect words within a text, latex or manual document.

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

## Installation

Binary packages for __OS-X__ or __Windows__ can be installed directly from CRAN:

```r
install.packages("hunspell")
```

Installation from source on Linux or OSX requires [`hunspell`](https://hunspell.github.io/). On __Debian or Ubuntu__ install [libhunspell-dev](https://packages.debian.org/testing/libhunspell-dev):

```
sudo apt-get install -y libhunspell-dev
```

On __Fedora__,  __CentOS or RHEL__ we need [hunspell-devel](https://apps.fedoraproject.org/packages/hunspell-devel):

```
sudo yum install hunspell-devel
````

On __OS-X__ use [hunspell](https://github.com/Homebrew/homebrew-core/blob/master/Formula/hunspell.rb) from Homebrew:

```
brew install hunspell
```

See the R `?hunspell` manual page for information on how to install additional dictionaries.
