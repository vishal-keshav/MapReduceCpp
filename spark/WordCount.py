from pyspark import SparkContext, SparkConf

if __name__ == "__main__":

    # create Spark context with Spark configuration
    conf = SparkConf().setAppName("Word Count")
    sc = SparkContext(conf=conf)

    # read input file and split each document into words
    words = sc.textFile('../data/WordCounterInput.txt').flatMap(lambda line: line.split(" "))

    # count the occurrence of each word
    wordCounts = words.map(lambda word: (word, 1)).reduceByKey(lambda v1, v2:v1+v2)
    word_list = wordCounts.collect()

    # write output in a text file
    output = list()
    output.append(format(word_list))
    f = open("../build/WordCount_SparkOutput.txt", "w")
    for each in output:
        f.write(each)
    f.close()