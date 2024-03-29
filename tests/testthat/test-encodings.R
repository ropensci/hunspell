context("Test UTF8 dict")

# Test that we can find a dictionary by full path
test_that("Dictionaries are by path",{
  expect_equal(hunspell_info(paste0(system.file("dict", package = "hunspell"), "/en_US"))$encoding, "UTF-8")
  expect_equal(hunspell_info("../testdict/ru_RU")$encoding, "UTF-8")
  expect_equal(hunspell_info("../testdict/russian-aot")$encoding, "KOI8-R")
})

# DICPATH gets preference, even when a system 'ru_RU' dict is found
Sys.setenv(DICPATH=normalizePath("../testdict", mustWork = TRUE))

test_that("Dictionaries are found",{
  expect_equal(hunspell_info("en_US")$encoding, "UTF-8")
  expect_equal(hunspell_info("ru_RU")$encoding, "UTF-8")
  expect_equal(hunspell_info("en_US")$wordchars, "\u2019")
  expect_equal(hunspell_info("en_GB")$wordchars, "\u2019")
  expect_equal(hunspell_info("ru_RU")$wordchars, NA_character_)
  expect_equal(hunspell_info("russian-aot")$encoding, "KOI8-R")
  expect_equal(nchar(hunspell_info("russian-aot")$wordchars), 122)
})

# needed because testthat doesn't parse UTF8 source code on windows
utf8 <- function(x){
  Encoding(x) <- "UTF-8"
  x
}

test_that("UTF8 always works", {
  str1 <- utf8("К сожалению, мне (нам) пора идти")
  str2 <- utf8("сожалению")
  str3 <- utf8("달력")
  str4 <- paste(str1, str3)
  expect_equal(length(hunspell_find(str1)[[1]]), 6)
  expect_equal(hunspell_find(str2)[[1]], str2)
  expect_equal(length(hunspell_find(str1, dict = "ru_RU")[[1]]), 0)
  expect_equal(hunspell_find(str4, dict = "ru_RU")[[1]], str3)
  expect_equal(hunspell_find(str4, ignore = str3, dict = "ru_RU")[[1]], character(0))
  expect_equal(length(hunspell_find(str1, dict = "russian-aot")[[1]]), 0)
  expect_equal(hunspell_find(str3, dict = "en_US")[[1]], str3)
  expect_equal(hunspell_find(str3, ignore = str3, dict = "en_US")[[1]], character(0))
  expect_equal(hunspell_parse(str4)[[1]][2], str2)
  expect_equal(hunspell_parse(str4)[[1]][7], str3)

  str5 <- utf8("Zürich")
  expect_equal(hunspell_parse(enc2native(paste(str5, "test")))[[1]][1], str5)
  expect_equal(hunspell_parse(enc2utf8(paste(str5, "test")))[[1]][1], str5)

  # Test encoding roundtrip
  stemstr <- utf8("сожалени")
  expect_match(hunspell_stem(str2, dict = "russian-aot")[[1]][1], stemstr)
  expect_match(hunspell_stem(str2, dict = "ru_RU")[[1]][1], stemstr)
  expect_match(hunspell_suggest(str2, dict = "russian-aot")[[1]][1], stemstr)
  expect_match(hunspell_suggest(str2, dict = "ru_RU")[[1]][1], stemstr)
  expect_match(hunspell_analyze(str2, dict = "russian-aot")[[1]][1], stemstr)
  expect_match(hunspell_analyze(str2, dict = "ru_RU")[[1]][1], stemstr)

  # iconv on macos 14.0 was broken, returns '?' instead of error. Fixed in 14.1
  #skip_on_os('mac')
  expect_warning(hunspell_find(str3, dict = "russian-aot"), "encoding")
})

test_that("custom words in dictionary",{
  word_native <-  enc2native("üx")
  word_utf <- enc2utf8(word_native)
  expect_false(hunspell_check(word_native))
  expect_false(hunspell_check(word_utf))
  expect_true(hunspell_check(word_native, dictionary(add_words = word_native)))
  expect_true(hunspell_check(word_utf, dictionary(add_words = word_utf)))
  expect_true(hunspell_check(word_native, dictionary(add_words = word_utf)))
  expect_true(hunspell_check(word_utf, dictionary(add_words = word_native)))

  # tests caching
  expect_false(hunspell_check(word_native))
  expect_false(hunspell_check(word_utf))
})
