#include "word_embeddings.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 *
 */
WordEmbeddings::WordEmbeddings() :
    _cb(nullptr), _init(false), _modelFile() // relative path from /path/to/repo/vector-space-retrieval
{}

/**
 * @brief Destroy the Document Manager:: Document Manager object
 *
 */
WordEmbeddings::~WordEmbeddings() {}

void WordEmbeddings::init(const control_block_t& aControlBlock, const std::string& aModelFile) {
    if (!_init) {
        _cb = &aControlBlock;
        _modelFile = aModelFile;
        loadModel(_modelFile);
        _init = true;
    }
}
