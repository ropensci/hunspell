context("Test UTF8 dict")

Sys.setenv(DICPATH=normalizePath("../testdict", mustWork = TRUE))

expect_that("Dictionaries are found",{
  expect_equal(hunspell_info("en_US")$encoding, "UTF-8")
  expect_equal(hunspell_info("ru_RU")$encoding, "UTF-8")
  expect_equal(hunspell_info("russian-aot")$encoding, "KOI8-R")
})

expect_that("UTF8 always works", {
  expect_equal(length(hunspell_find("К сожалению, мне (нам) пора идти")[[1]]), 6)
  expect_equal(hunspell_find("сожалению")[[1]], "сожалению")
  expect_equal(length(hunspell_find("К сожалению, мне (нам) пора идти", dict = "ru_RU")[[1]]), 0)
  expect_equal(hunspell_find("К сожалению, мне (нам) пора идти  달력", dict = "ru_RU")[[1]], "달력")
  expect_equal(length(hunspell_find("К сожалению, мне (нам) пора идти", dict = "russian-aot")[[1]]), 0)
  expect_equal(hunspell_find("달력", dict = "en_US")[[1]], "달력")
  expect_warning(hunspell_find("달력", dict = "russian-aot"), "encoding")
})
