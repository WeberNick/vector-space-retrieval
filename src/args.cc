#include "args.hh"

void construct_arg_desc(argdesc_vt& x) {
    // using carg_t = argdesc_t<Args, char>;
    // using iarg_t = argdesc_t<Args, int>;
    using uarg_t = argdesc_t<Args, uint>;
    // using farg_t = argdesc_t<Args, float>;
    // using darg_t = argdesc_t<Args, double>;
    using barg_t = argdesc_t<Args, bool>;
    using sarg_t = argdesc_t<Args, std::string>;

    x.push_back(new barg_t("--help", false, &Args::help, "print this message"));
    x.push_back(new barg_t("--trace", false, &Args::trace, "sets the flag to activate tracing"));
    x.push_back(new barg_t("--measure", false, &Args::measure, "sets the flag to measure performance"));
    x.push_back(new barg_t("--server", false, &Args::server, "sets the flag to indicate a server connection"));

    x.push_back(new sarg_t("--collection-path", "./data/d-collection.docs", &Args::collectionPath, "path to the collection file. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--query-path", "./data/", &Args::queryPath, "path to the query directory. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--scores-path", "./data/s-3.qrel", &Args::relevanceScoresPath, "path to the relevance scores file. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--stopword-path", "./data/stopwords.large", &Args::stopwordPath, "path to the stopwords file. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--word-embeddings-path", "./data/w2v/glove.6B.300d.txt", &Args::wordEmbeddingsPath, "path to the word embeddings file. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--trace-path", "./", &Args::tracePath, "path to the log directory. relative path from repo root directory or absolute path possible"));
    x.push_back(new sarg_t("--eval-path", "./", &Args::evalPath, "path to the evaluation directory. relative path from repo root directory or absolute path possible"));

    x.push_back(new uarg_t("--topk", 20, &Args::results, "the maximum number of results a query shall return (top k)"));
    x.push_back(new uarg_t("--tiers", 50, &Args::tiers, "the number of tiers used for the tiered index"));
    x.push_back(new uarg_t("--dimensions", 1000, &Args::dimensions, "the number of dimensions used for the random projection"));
    x.push_back(new uarg_t("--seed", 1, &Args::seed, "seed for random projection and selecting the cluster leaders"));

    /*REMOVE*/
    x.push_back(new barg_t("--rand", false, &Args::rand, "print this message"));
    x.push_back(new barg_t("--tiered", false, &Args::tiered, "print this message"));
}

Args::Args() : 
    _help(false),
    _trace(false),
    _measure(false),
    _server(false),
    _collectionPath("./data/d-collection.docs"),
    _queryPath("./data/"),
    _relScoresPath("./data/s-3.qrel"),
    _stopwordPath("./data/stopwords.large"),
    _wordEmbeddingsPath("./data/w2v/glove.6B.300d.txt"),
    _tracePath("./"),
    _evalPath("./"),
    _results(20),
    _tiers(50),
    _dimensions(1000),
    _seed(1),
    _rand(false),
    _tiered(false)
{}
