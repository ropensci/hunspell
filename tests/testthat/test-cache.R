context("Dictionary Cache")

test_that("Caching works as expected", {
  # Clean cache
  rm(list = ls(store), pos = store)
  expect_length(ls(store), 0)

  # Load two dictionaries
  us <- dictionary('en_US')
  gb <- dictionary('en_GB')
  expect_length(ls(store), 2)

  # Load again
  us <- dictionary('en_US')
  gb <- dictionary('en_GB')
  expect_length(ls(store), 2)

  # Add words
  us <- dictionary('en_US', add_words = 'jeroen')
  us <- dictionary('en_US', add_words = c('jeroen', 'jeroen'))
  expect_length(ls(store), 3)
  us <- dictionary('en_US', add_words = 'erik')
  expect_length(ls(store), 4)

  us <- dictionary('en_GB', add_words = 'jeroen')
  expect_length(ls(store), 5)
})

test_that("No false positives", {
  expect_length(hunspell('jeroen', dict = 'en_US', ignore = NULL)[[1]], 1)
  expect_length(hunspell('jeroen', dict = 'en_US', ignore = 'jeroen')[[1]], 0)
  expect_length(hunspell('jeroen', dict = 'en_US', ignore = NULL)[[1]], 1)
  expect_length(ls(store), 5)

  expect_length(hunspell('jeroen', dict = 'en_US', ignore = c("erik", "jeroen"))[[1]], 0)
  expect_length(hunspell('jeroen', dict = 'en_US', ignore = c("jeroen", "erik"))[[1]], 0)
  expect_length(ls(store), 6)
})
