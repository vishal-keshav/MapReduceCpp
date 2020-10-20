from pyspark import SparkContext, SparkConf

if __name__ == "__main__":

    # create Spark context with Spark configuration
    conf = SparkConf().setAppName("Word Count")
    sc = SparkContext(conf=conf)

    # read input file, split source,targets and return list of (target,source)
    def get_target_source(line):
        source,target = line.split(',')
        return target, [source]

    target_sources = sc.textFile('../data/website_source_targets.txt').map(lambda line: get_target_source(line))

    # reduce the source lists by the key (target) they pointed to
    target_all_sources = target_sources.reduceByKey(lambda v1, v2:v1+v2)
    collected_target_all_sources = target_all_sources.collect()

    # write output in a text file
    output_file_loc="../build/ReverseWeblinkGraph_SparkOutput.txt"
    with open(output_file_loc, 'w') as file:
        for t,s in collected_target_all_sources:
            file.write(f'{t} {" ".join(s)}\n')
    print(f'file successfully saved to {output_file_loc}')