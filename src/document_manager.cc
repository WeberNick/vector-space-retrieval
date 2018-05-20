#include "document_manager.hh"

/**
 * @brief Construct a new Document Manager:: Document Manager object
 *
 */
DocumentManager::DocumentManager() :
    _cb(nullptr),
    _delimiter('~'),
    _docs(),
    _docids(),
    _str_docid()
{}

void DocumentManager::init(const control_block_t& aControlBlock) {
    if (!_cb) {
        _cb = &aControlBlock;
        const std::string& lCollectionPath = _cb->collectionPath();
        string_vvt lCollectionFiles;
        Util::readIn(lCollectionPath, _delimiter, lCollectionFiles);
        for (auto& line : lCollectionFiles) {
            std::string lDocID = line.at(0);
            string_vt lContent;
            Util::splitStringBoost(line.at(1), ' ', lContent); // Split string by whitespaces
            Document doc(lDocID, lContent);
            addDoc(doc);
        }
        TRACE("DocumentManager: Initialized");
    }
}