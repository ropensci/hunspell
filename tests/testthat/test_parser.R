context("Tokenizer")

test_that("Parser works for large strings",{
  x <- "I want to see the lovely, handier version of sing or singing dogs"
  y <- paste(rep(x, 100), collapse=" ")
  for(i in 1:10) {
    expect_length(hunspell_parse(y)[[1]], 1300)
    expect_length(hunspell_parse(y, dict = "ru_RU")[[1]], 1300)
    expect_length(hunspell_parse(y, dict = "russian-aot")[[1]], 1300)
  }
})

# contractions and posessive are the most challenging to check
test_that("Test English apostrophe",{
  text <- c(
    "let's, don't, couldn't, it's, she's",
    "the lawyer's fee",
    "the child's toy",
    "children's toys",
    "Tom Jones's first album",
    "anyone's guess",
    "excessive lawyers' fees",
    "the twins' parents",
    "the student teachers' supervisor",
    "the boys' baseball team"
  )

  # Do not split at the apostrophe
  len <- c(5, 3, 3, 2, 4, 2, 3, 3, 4, 4)
  expect_equal(sapply(hunspell_parse(text, dict = 'en_US'), length), len)
  expect_equal(sapply(hunspell_parse(text, dict = 'en_GB'), length), len)

  # Do not find any typos
  lapply(text, function(x){
    expect_length(hunspell(x, dict = 'en_GB')[[1]], 0)
    expect_length(hunspell(x, dict = 'en_US')[[1]], 0)
  })
})

test_that("Quotes around words are ignored", {
  str <- "The 'hunspell' library"
  words <- c("The", "hunspell", "library")
  expect_equal(hunspell_parse(str, dict = 'en_US')[[1]], words)
  expect_equal(hunspell_parse(str, dict = 'en_GB')[[1]], words)
  expect_equal(hunspell(str, dict = 'en_US')[[1]], "hunspell")
  expect_equal(hunspell(str, dict = 'en_GB')[[1]], "hunspell")
})
