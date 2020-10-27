import os
#import shutil
from pyspark import SparkContext, SparkConf

if __name__ == "__main__":

     # read the input file (in /data) and write each line to a separate file (write to /tmp)
    f = open("data/InvertedIndexInput.txt")
    lines = f.readlines()
    f.close()
    line_cnt = 0
    try:
        os.mkdir('tmp')
    except OSError:
        print('creation of directory failed. tmp already exist')  
    for line in lines:
        line_cnt = line_cnt+1
        # name the file as the number of the line
        f = open("tmp/%s.txt" % line_cnt, "w+")
        f.write(line)
        f.close()

    # create spark context
    conf = SparkConf().setAppName("Inverted Index")
    sc = SparkContext(conf=conf)
    
    # read the files in /tmp into tuples (filename, line)
    txtfiles = sc.wholeTextFiles('tmp')

    # flatmap the tuples to (parsed_filename, word)
    # filename is /Users/user/.../spark/tmp/line_num.txt <-- need to parse into line_num
    fm = txtfiles.flatMap(lambda t: ((t[0][-5], word) for word in t[1].split()))

    # map: (parsed_filename, word) --> (word, parsed_filename)
    mp = fm.map(lambda u: (u[1], u[0]))

    # group line_nums with same word (key)
    reduced = mp.reduceByKey(lambda a, b: a+" "+b)

    # delete tmp directory
    #shutil.rmtree('tmp', ignore_errors=True)

    # write output to file
    ls = reduced.collect()
    output = list()
    output.append(format(ls))
    f = open("build/InvertedIndexData/InvertedIndex_SparkOutput.txt", "w")
    for each in output:
        f.write(each)
    f.close()