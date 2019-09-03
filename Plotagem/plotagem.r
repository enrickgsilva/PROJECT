par(mfrow=c(1,2))

list_c <- read.csv("AVL.csv", header=TRUE, sep="\t")
plot(list_c$size, ylab="Comparações", xlab="Tamanho", main="AVL",xlim=c(0,1000),bty="l", tcl=0.3, col = "magenta")

list_c <- read.csv("ABB.csv", header=TRUE, sep="\t")
plot(list_c$size, ylab="Comparações", xlab="Tamanho", main="ABB",bty="l", tcl=0.3, col = "cyan")