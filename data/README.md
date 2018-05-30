# Contents

## Original Files

* `dev/raw`         - the raw dev set files from nfcorpus
* `test/raw`        - the raw test set files from nfcorpus
* `train/raw`       - the raw training set files from nfcorpus
* `w2v`             - the directory containing the word2vec model for word embeddings, we used glove.6B.300d.txt from [GloVe](https://nlp.stanford.edu/projects/glove/)
* `stopwords.large` - contains stopwords

## Processed File Structure

* `q-{all, nontopictitles, titles, viddesc, vidtitles}.queries` - different types of queries for which the relevant documents in `d-collection.docs` are listed in the `s-3.qrel` file. The queries are not yet preprocessed. Format: `QUERY_ID~TEXT`
* `d-collection.docs` - document collection (lowercased, stop-word filtered, numbers removed, no punctuation, tokenized, stemmed). Format: `DOC_ID~TEXT`
* `s-3.qrel`          - files containing relevance of documents for a query. Format: `QUERY_ID~ DOC_ID~RELEVANCE`, where the relevance score is between 0, 1 and 2
