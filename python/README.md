# Python Scripts
* preprocessing/: The preprocess.py module provides methods to preprocess the data, namely queries and documents (for more information on the data see [data/](#data/). Basically, the queries and documents are tokenized, stemmed and stopwords are removed (according to the [stopwords.large](/data/stopwords.large) file from nfcorpus). Queries for which there are no query relevance score in the .qrel files are disregarded. To run it, you have to install the virtual environment using pipenv and run it afterwards:
```
~python/preprocessing$ pipenv install
~python/preprocessing$ pipenv run python preprocess.py
```
* wiki-crawler/: The wiki-crawler was an attempt to crawl all medicine related Wikipedia pages for building a corpus which then could have been used to build domain specific word embeddings. It should have been starting at the portal page for medicine and then following each link it finds. But this procedure was never implemented nor fully used. Due to time constraints we decided to use a pre-computed word embeddings model from the [GloVe project](https://nlp.stanford.edu/projects/glove/).
