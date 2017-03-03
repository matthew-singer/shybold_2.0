read_dir <- function(dirname) {
  file_names <- dir(dirname, pattern="pred.*", full.names = T)
  d.all <- NA
  rep <- 1
  for (f in file_names) {
    print(f)
    d <- read.table(f, header=T, sep="\t")
    d$Rep <- rep
    d.all <- rbind(d.all, d)
    rep = rep + 1
  }
  d.all <- d.all[-1, ]
  return(d.all)  
}
s <- read_dir("~/Present/data/simple/")
