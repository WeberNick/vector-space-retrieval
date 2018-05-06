''' Preprocessing module '''
import re
import string

from natsort import natsorted
from nltk.stem.snowball import SnowballStemmer

DATA_PATH = '../../data'
RAW_FOLDER = 'raw'

def qrel(folders, files, end):
    ''' Aggregate query relevance '''
    for folder in folders:
        for filename in files:
            result = set()
            with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end)) as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    del content[1]
                    result.add('~'.join(content))
            _write(result, '{}/{}/{}.{}'.format(DATA_PATH, folder, filename, end))

def docs(folders, filename, end):
    ''' Aggregate document collection '''
    stemmer = SnowballStemmer(language='english')
    regex = re.compile('[%s]' % re.escape(string.punctuation))
    for folder in folders:
        result = set()
        with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end)) as file_from:
            for line in file_from.readlines():
                content = re.split(r'\t+', line.rstrip('\t\n'))
                words = [regex.sub('', w) for w in content[1].split()]
                words = [stemmer.stem(w) for w in words]
                words = [w for w in words if len(w) > 1]
                doc = [content[0], ' '.join(words)]
                result.add('~'.join(doc))
        _write(result, '{}/{}/{}.{}'.format(DATA_PATH, folder, filename, end))

def quer(folders, files, end):
    ''' Aggregate queries '''
    for folder in folders:
        for filename in files:
            result = set()
            with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end)) as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    result.add('~'.join(content))
            _write(result, '{}/{}/{}.{}'.format(DATA_PATH, folder, filename, end))

def _write(result_set, filename):
    ''' Write results to a file '''
    with open(filename, 'w') as file_to:
        for line in natsorted(list(result_set), key=lambda x: x.lower()):
            file_to.write('{}\n'.format(line))

if __name__ == '__main__':
    folders = ['test', 'dev', 'train']
    qrel(folders, ['s-3', 's-4'], 'qrel')
    docs(folders, 'd-collection', 'docs')
    quer(folders, ['q-all', 'q-titles', 'q-nontopictitles', 'q-vidtitles', 'q-viddesc'], 'queries')
