context("Test Case Sensitivity")

# By default, only 'JavaScript' is correct case

test_that("Words have proper casing", {
  expect_true(hunspell_check("JavaScript", dict = 'en_GB'))
  expect_true(hunspell_check("JavaScript", dict = 'en_US'))
  expect_false(hunspell_check("Javascript", dict = 'en_GB'))
  expect_false(hunspell_check("Javascript", dict = 'en_US'))

  # Add the word
  expect_true(hunspell_check("JavaScript", dict = dictionary(lang = 'en_GB', add_words = "Javascript")))
  expect_true(hunspell_check("JavaScript", dict = dictionary(lang = 'en_US', add_words = "Javascript")))
  expect_true(hunspell_check("Javascript", dict = dictionary(lang = 'en_GB', add_words = "Javascript")))
  expect_true(hunspell_check("Javascript", dict = dictionary(lang = 'en_US', add_words = "Javascript")))

  # This is still false
  expect_false(hunspell_check("JAVAScript", dict = dictionary(lang = 'en_GB')))
  expect_false(hunspell_check("JAVAScript", dict = dictionary(lang = 'en_US')))
  expect_false(hunspell_check("JAVAScript", dict = dictionary(lang = 'en_GB', add_words = "Javascript")))
  expect_false(hunspell_check("JAVAScript", dict = dictionary(lang = 'en_US', add_words = "Javascript")))

})
