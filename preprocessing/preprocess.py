''' Preprocessing module '''
import re

from natsort import natsorted
from nltk.stem.snowball import SnowballStemmer

DATA_PATH = '../data/'
CORPUS_PATH = '../data/nfcorpus/' # location of unprocessed corpus; not pushed to remote server

def qrel(files, ending):
    ''' Aggregate query relevance '''
    for end in ending:
        result = set()
        for filename in files:
            with open(CORPUS_PATH + filename + '.' + end) as relevance_file:
                for line in relevance_file.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    del content[1]
                    result.add('~'.join(content))
        _write(result, end)

def docs(files, end):
    ''' Aggregate document collection '''
    stemmer = SnowballStemmer(language='english')
    result = set()
    for filename in files:
        with open(CORPUS_PATH + filename + '.' + end) as f:
            for line in f.readlines():
                content = re.split(r'\t+', line.rstrip('\t\n'))
                words = ' '.join([stemmer.stem(word) for word in content[1].split()])
                doc = [content[0], words]
                result.add('~'.join(doc))
    _write(result, 'collection.{}'.format(end))

def _write(result_set, filename):
    ''' Write results to a file '''
    with open(DATA_PATH + filename, 'w') as file_to:
        for line in natsorted(list(result_set), key=lambda x: x.lower()):
            file_to.write('{}\n'.format(line))

if __name__ == '__main__':
    qrel(['test', 'dev', 'train'], ['3-2-1.qrel', '2-1-0.qrel'])
    docs(['test', 'dev', 'train'], 'docs')
