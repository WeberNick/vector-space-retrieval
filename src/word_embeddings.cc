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

int WordEmbeddings::loadModel(const std::string& aModelFile) {
    // load wor2vec model
    try {
        _w2vModel.reset(new w2v::w2vModel_t());
        // load w2v model file
        if (!_w2vModel->load(aModelFile)) { throw std::runtime_error(_w2vModel->errMsg()); }
    } catch (const std::exception& _e) {
        std::cerr << _e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 2;
    }
}
