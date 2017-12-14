context("parser")

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
    "anyone's guess"
  )

  # Do not split at the apostrophe
  expect_equal(sapply(hunspell_parse(text, dict = 'en_US'), length), c(5, 3, 3, 2, 4, 2))
  expect_equal(sapply(hunspell_parse(text, dict = 'en_GB'), length), c(5, 3, 3, 2, 4, 2))

  # Do not find any typos
  lapply(text, function(x){
    expect_length(hunspell(x, dict = 'en_GB')[[1]], 0)
    expect_length(hunspell(x, dict = 'en_US')[[1]], 0)
  })

  # These only work in the GB dictionary it seems:
  text <- c(
    "excessive lawyers' fees",
    "the twins' parents",
    "the student teachers' supervisor",
    "the boys' baseball team"
  )
  lapply(text, function(x){
    expect_length(hunspell(x, dict = 'en_GB')[[1]], 0)
    #expect_length(hunspell(x, dict = 'en_US')[[1]], 0)
  })
})
