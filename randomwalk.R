#install.packages("Rcpp") # uncomment if not installed
library(Rcpp)
sourceCpp("ranwalk.cpp")

wa <- walk2(10,3)

#---------
#Question 1
#---------

#Has the destination [1,-3] reached after walking a 100 roads? returns a vector of Bool, 
#as destination might have been reached twice. Therefore TRUE might have come up twice
TRUE%in%Destination(as.integer(c(-1,3)),100,seed = 4) #yes destination has been reached
#by returning a vector of bools we can find out at which position the destination was reached
which(Destination(as.integer(c(-1,3)),100,seed = 4)==TRUE) #It has been reached after walking 69 roads and again afer 71 

#---------
#Question2
#---------

yes20 = rep(0,1000) #stores 1 if reached within 20 and 0 if not
for(i in 1:1000){
  walks = Destination(as.integer(c(1,-3)),20,seed = i)
  #
  if(TRUE%in%walks){
    yes20[i] = 1
  
  }
}
su <- sum(yes20)/1000 #probability is 0.11 of reaching destination within walking 20 roads

#----------
#Question 3
#----------
#compare the two walk functions
#library(rbenchmark)
#benchmark(walk(100,2),walk2(100,2),replications(100),order = "relative") 
#error using benchmark error so it wont work - "Error in formula.default(object, env = baseenv()) : invalid formula"
#-------
#Using microbenchmark instead
library(microbenchmark)
ben <- microbenchmark(mod2 = {walk0(1000,2)}, mod4 = {walk1(1000,2)}, ra16 = {walk2(1000,2)}, times = 100, unit = "milliseconds")
#interesting to see my second walk function is more efficient
#------

#----------
#Question 4
#----------

#Manhattan distance
avgD <- c()
for(i in 1:1000){
  avgD<-c(avgD,ManhattanD(50,i))
}
summary(avgD)
mean(avgD) # = 7.698 for walk2 
