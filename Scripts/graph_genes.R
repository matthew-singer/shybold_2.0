library(ggplot2)
library(reshape2)
library(cowplot)
library(corrplot)

plot_genes <- function(file_names, title = "Gene Weights") {
  d.all <- NA
  rep <- 1
  for (f in file_names) {
    print(f)
    d <- read.table(f, header=T, sep="\t")
    d.agg <- aggregate(d, by=list(d$Gen), FUN=mean)
    d.agg$Fitness <- NULL
    d.agg$Group.1 <- NULL
    d.agg$Index <- NULL
    #d.agg$Gene17 <- NULL
    d.agg$Metabolic <- NULL
    #d.agg$Radius <- NULL
    #d.agg$Gene8 <- NULL
    d.agg$Gametes <- NULL
    d.melted <- melt(d.agg, id.vars="Gen")
    d.melted$rep <- rep
    
    rep <- rep + 1
    
    d.all <- rbind(d.all, d.melted)
  }
  d.all <- d.all[-1, ]
  
  g <- ggplot(data = d.all , aes(Gen, value)) + 
    geom_line(aes(colour=factor(rep)))
    facet_wrap(~variable) + ggtitle(title)
  g
}
plot_genes(dir(path = ".", pattern = "pred.*exp.*", full.names = T))



corr_genes <- function(f) {
  d <- read.table(f, header=T, sep="\t")
  #d <- d[d$Gen == max(d$Gen),]
  d.agg <- aggregate(d, by=list(d$Gen), FUN=mean)
  d.agg$Group.1 <- NULL
  d.agg$Index <- NULL
  
  d.agg_cors <- cor(d.agg)
  #cors <- cors[,2:12]
  #cors <- cors[2:12,]
  #png(paste("Gen_Averages_Pred_Corr_Rep_", i, ".png", sep=""))
  corrplot(d.agg_cors, method="ellipse", type="lower", title=paste("Gen Averages Rep", basename(f)))
  #dev.off()
}

corr_genes("./prey_1485892029063_brutal_selection_2")

plot_genes(dir(path = "~/Shybold/", pattern = "pred_", full.names = T))



#library(devtools)
#install_github("ggbiplot", "vqv")

library(ggbiplot)
g <- ggbiplot(z, obs.scale = 1, var.scale = 1, 
               ellipse = TRUE, 
              circle = TRUE)
g <- g + scale_color_discrete(name = '')
g <- g + theme(legend.direction = 'horizontal', 
               legend.position = 'top')
print(g)


