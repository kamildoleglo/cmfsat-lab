library("ggplot2")

results = read.csv("results.csv")

ag = aggregate(TIME ~ SIZE:TIME:METHOD, results, function(x) c(mean = mean(x), deviation = sd(x)))
ag = do.call(data.frame, ag)

ggplot(ag,aes(SIZE,TIME.mean,color=METHOD)) + geom_point() + ylab("Computing time in seconds") + geom_errorbar(data = ag,mapping=aes(ymin=TIME.mean-TIME.deviation, ymax=TIME.mean+TIME.deviation))

fitn = lm(TIME.mean ~ poly(SIZE, 3, raw=TRUE), data=ag[ag[, "METHOD"]=="Naive",])
fitb = lm(TIME.mean ~ poly(SIZE, 3, raw=TRUE), data=ag[ag[, "METHOD"]=="Better",])
fitbl = lm(TIME.mean ~ poly(SIZE, 3, raw=TRUE), data=ag[ag[, "METHOD"]=="BLAS",])

newdata = data.frame(SIZE = seq(1,1000, length.out=500))
newdata$yn = predict(fitn, newdata)
newdata$yb = predict(fitb, newdata)
newdata$ybl = predict(fitbl, newdata)

last_plot() + geom_line(data=newdata, aes(SIZE, yn, color='Naive')) + geom_line(data=newdata, aes(SIZE,yb, color="Better")) + geom_line(data=newdata, aes(SIZE,ybl, color="BLAS"))


