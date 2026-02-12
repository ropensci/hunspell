context("Test hunspell_info detailed parameter")

test_that("hunspell_info works without detailed parameter", {
  info <- hunspell_info("en_US")
  expect_true("dict" %in% names(info))
  expect_true("affix" %in% names(info))
  expect_true("encoding" %in% names(info))
  expect_true("wordchars" %in% names(info))
  expect_true("added" %in% names(info))
  expect_false("version" %in% names(info))
  expect_false("wordcount" %in% names(info))
})

test_that("hunspell_info with detailed=FALSE returns basic info", {
  info <- hunspell_info("en_US", detailed = FALSE)
  expect_false("version" %in% names(info))
  expect_false("wordcount" %in% names(info))
})

test_that("hunspell_info with detailed=TRUE returns additional info", {
  info <- hunspell_info("en_US", detailed = TRUE)
  expect_true("version" %in% names(info))
  expect_true("wordcount" %in% names(info))
  expect_true(is.numeric(info$wordcount))
  expect_true(info$wordcount > 0)
})

test_that("wordcount is reasonable for en_US dictionary", {
  info <- hunspell_info("en_US", detailed = TRUE)
  # en_US dictionary should have thousands of words
  expect_true(info$wordcount > 1000)
})

test_that("version field is present and properly formatted", {
  info <- hunspell_info("en_US", detailed = TRUE)
  # version field should always be present when detailed=TRUE
  expect_true("version" %in% names(info))
  # version is either a character string or NA (if not defined in .aff file)
  expect_true(is.character(info$version) || is.na(info$version))
  # if version is a string, it should not be empty
  if(is.character(info$version) && !is.na(info$version)) {
    expect_true(nchar(info$version) > 0)
  }
})
