context("Test hunspell_info with version and wordcount")

test_that("hunspell_info returns all expected fields including version and wordcount", {
  info <- hunspell_info("en_US")
  expect_true("dict" %in% names(info))
  expect_true("affix" %in% names(info))
  expect_true("encoding" %in% names(info))
  expect_true("wordchars" %in% names(info))
  expect_true("added" %in% names(info))
  expect_true("version" %in% names(info))
  expect_true("wordcount" %in% names(info))
})

test_that("wordcount field is numeric and positive", {
  info <- hunspell_info("en_US")
  expect_true(is.numeric(info$wordcount))
  expect_true(info$wordcount > 0)
})

test_that("wordcount is reasonable for en_US dictionary", {
  info <- hunspell_info("en_US")
  # en_US dictionary should have thousands of words
  expect_true(info$wordcount > 1000)
})

test_that("version field is present and properly formatted", {
  info <- hunspell_info("en_US")
  # version field should always be present
  expect_true("version" %in% names(info))
  # version is either a character string or NA (if not defined in .aff file)
  expect_true(is.character(info$version) || is.na(info$version))
  # if version is a string, it should not be empty
  if(is.character(info$version) && !is.na(info$version)) {
    expect_true(nchar(info$version) > 0)
  }
})

