import sys
import re

from pyspark import SparkContext

def flatMapFunc(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document. We want
    a distinct list of all the words in each document to pass in to our map
    function, but if we call the transformation distinct() on all the words from
    all the documents with the current code, we won't get quite what we want. How
    can we use the document ID to help get around that problem?
    """
    """ Your code here. """
    return [(word, document[0])for word in  re.findall(r"\w+", document[1])]

def mapFunc(word_pair):
    """ Your code here. """
    return (word_pair[0], 1)

def reduceFunc(a, b):
    """ Your code here. """
    return a + b

def docwordcount(file_name, output="spark-wc-out-docwordcount"):
    sc = SparkContext("local[8]", "DocWordCount")
    file = sc.sequenceFile(file_name)

    counts = file.flatMap(flatMapFunc) \
                 .distinct() \
                 .map(mapFunc) \
                 .reduceByKey(reduceFunc)

    counts.coalesce(1).sortByKey().saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        docwordcount(argv[1])
    else:
        docwordcount(argv[1], argv[2])
