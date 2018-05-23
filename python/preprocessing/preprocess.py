''' Preprocessing module '''
import re
import string

from natsort import natsorted
from nltk.stem.snowball import SnowballStemmer

DATA_PATH = '../../data'
RAW_FOLDER = 'raw'

def qrel(folders, file, end):
    ''' Preprocess query relevance '''
    ids = set()
    #for filename in files:
    result = set()
    for folder in folders:
        with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, file, end), 'r') as file_from:
            for line in file_from.readlines():
                content = re.split(r'\t+', line.rstrip('\t\n'))
                ids.add(content[0])
                del content[1]
                result.add('~'.join(content))
    _write(result, '{}/{}.{}'.format(DATA_PATH, file, end))
    return ids

def docs(folders, filename, end):
    ''' Preprocess document collection '''
    stemmer = SnowballStemmer(language='english')
    regex = re.compile('[%s]' % re.escape(string.punctuation))
    with open('{}/{}'.format(DATA_PATH, 'stopwords.large'), 'r') as stopwordfile:
        stopwords = stopwordfile.readline().strip("\"").split(",")
    result = set()
    for folder in folders:
        with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end), 'r') as file_from:
            for line in file_from.readlines():
                content = re.split(r'\t+', line.rstrip('\t\n'))
                words = [regex.sub(' ', w) for w in content[1].split()]
                words = [stemmer.stem(w) for w in words]
                words = [w.strip() for w in words if len(w) > 1 and w not in stopwords]
                doc = [content[0], ' '.join(words)]
                result.add('~'.join(doc))
    _write(result, '{}/{}.{}'.format(DATA_PATH, filename, end))

def quer(folders, files, end, qrelids):
    ''' Preprocess queries '''
    stemmer = SnowballStemmer(language='english')
    regex = re.compile('[%s]' % re.escape(string.punctuation))
    with open('{}/{}'.format(DATA_PATH, 'stopwords.large'), 'r') as stopwordfile:
        stopwords = stopwordfile.readline().strip("\"").split(",")
    for filename in files:
        result = set()
        for folder in folders:
            with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end), 'r') as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    if not content[0] in qrelids:
                        print("Skipped: " + content[0])
                        continue
                    words = [regex.sub(' ', w) for w in content[1].split()]
                    words = [stemmer.stem(w) for w in words]
                    words = [w.strip() for w in words if len(w) > 1 and w not in stopwords]
                    doc = [content[0], ' '.join(words)]
                    result.add('~'.join(doc))
        _write(result, '{}/{}.{}'.format(DATA_PATH, filename, end))

def quer_no_preprocessing(folders, files, end, qrelids):
    ''' Preprocess queries '''
    stemmer = SnowballStemmer(language='english')
    regex = re.compile('[%s]' % re.escape(string.punctuation))
    with open('{}/{}'.format(DATA_PATH, 'stopwords.large'), 'r') as stopwordfile:
        stopwords = stopwordfile.readline().strip("\"").split(",")
    for filename in files:
        result = set()
        for folder in folders:
            with open('{}/{}/{}/{}.{}'.format(DATA_PATH, folder, RAW_FOLDER, filename, end), 'r') as file_from:
                for line in file_from.readlines():
                    content = re.split(r'\t+', line.rstrip('\t\n'))
                    if not content[0] in qrelids:
                        print("Skipped: " + content[0])
                        continue
                    result.add('~'.join(content))
        _write(result, '{}/{}.{}'.format(DATA_PATH, filename, end))

def _write(result_set, filename):
    ''' Write results to a file '''
    with open(filename, 'w') as file_to:
        for line in natsorted(list(result_set), key=lambda x: x.lower()):
            file_to.write('{}\n'.format(line))

if __name__ == '__main__':
    folders = ['test', 'dev', 'train']
    qrelids = qrel(folders, 's-3', 'qrel')
    docs(folders, 'd-collection', 'docs')
    quer_no_preprocessing(folders, ['q-all', 'q-titles', 'q-nontopictitles', 'q-vidtitles', 'q-viddesc'], 'queries', list(qrelids))
