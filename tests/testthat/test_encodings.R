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
  expect_warning(hunspell_find(str3, dict = "russian-aot"), "encoding")
  expect_equal(hunspell_parse(str4)[[1]][2], str2)
  expect_equal(hunspell_parse(str4)[[1]][7], str3)

  str5 <- utf8("Zürich")
  expect_equal(hunspell_parse(enc2native(paste(str5, "test")))[[1]][1], str5)
  expect_equal(hunspell_parse(enc2utf8(paste(str5, "test")))[[1]][1], str5)

})
