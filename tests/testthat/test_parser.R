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
