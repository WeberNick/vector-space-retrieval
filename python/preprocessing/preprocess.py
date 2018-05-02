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
            with open(CORPUS_PATH + filename + '.' + end) as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    del content[1]
                    result.add('~'.join(content))
        _write(result, end)

def docs(files, end):
    ''' Aggregate document collection '''
    stemmer = SnowballStemmer(language='english')
    result = set()
    for filename in files:
        with open(CORPUS_PATH + filename + '.' + end) as file_from:
            for line in file_from.readlines():
                content = re.split(r'\t+', line.rstrip('\t\n'))
                words = ' '.join([stemmer.stem(word) for word in content[1].split()])
                doc = [content[0], words]
                result.add('~'.join(doc))
    _write(result, 'collection.{}'.format(end))

def quer(files, types, end):
    ''' Aggregate queries '''
    for typ in types:
        result = set()
        for filename in files:
            with open('{}{}.{}.{}'.format(CORPUS_PATH, filename, typ, end)) as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    result.add('~'.join(content))
        _write(result, '{}.{}'.format(typ, end))

def _write(result_set, filename):
    ''' Write results to a file '''
    with open(DATA_PATH + filename, 'w') as file_to:
        for line in natsorted(list(result_set), key=lambda x: x.lower()):
            file_to.write('{}\n'.format(line))

if __name__ == '__main__':
    shape = ['test', 'dev', 'train']
    #qrel(shape, ['3-2-1.qrel', '2-1-0.qrel'])
    #docs(shape, 'docs')
    quer(shape, ['all', 'titles', 'nontopic-titles', 'vid-titles', 'vid-desc'], 'queries')
