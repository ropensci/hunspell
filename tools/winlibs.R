# Build against mingw-w64 build of hunspell 1.3.3
if(!file.exists("../windows/hunspell-1.3.3/include/hunspell/hunspell.hxx")){
  if(getRversion() < "3.3.0") setInternet2()
  download.file("https://github.com/rwinlib/hunspell/archive/v1.3.3.zip", "lib.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("lib.zip", exdir = "../windows")
  unlink("lib.zip")
}
