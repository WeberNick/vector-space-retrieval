# Contents

* `{all, nontopic-titles, titles, vid-desc, vid-titles}.queries` - different types of queries for which the relevant documents in `collection.docs` are listed in the `.qrel` files. The queries are not yet preprocessed. Format: `QUERY_ID ~ TEXT`
* `collection.docs`     - document collection (lowercased, stop-word filtered, numbers removed, no punctuation, tokenized, stemmed). Format: `DOC_ID ~ TEXT`
* `{3-2-1, 2-1-0}.qrel` - files containing relevance of documents for a query. Format: `QUERY_ID ~ DOC_ID ~ RELEVANCE`
 * __NICO DESCRIBE THIS__
 * dev/raw: 
 * test/raw: 
 * train/raw: 
 * collection\_test.docs:
 * collection\_test\_mwe.docs:
 * d-collection.docs:
 * q-all.queries:
 * q-nontopictitles.queries:
 * q-titles.queries:
 * q-viddesc.queries:
 * q-vidtitles.queries:
 * s-3.qrel:
 * stopwords.large:
