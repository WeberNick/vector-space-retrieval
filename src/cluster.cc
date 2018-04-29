#include "cluster.hh"

/**
 * @brief Construct a new Cluster:: Cluster object
 *
 */
Cluster::Cluster() : _cb(nullptr), _init(false), _leaders(), _cluster() {}

/**
 * @brief Destroy the Cluster:: Cluster object
 *
 */
Cluster::~Cluster() {}

void Cluster::init(const CB& aControlBlock) {
    _cb = &aControlBlock;
    if (!_init) {
        chooseLeaders();
        fillCluster();
        _init = true;
        TRACE("Cluster: Initialized.");
    }
}

void Cluster::chooseLeaders() {
    const doc_mt& lDocs = DocumentManager::getInstance().getDocumentMap();
    std::random_device lSeeder;                               // the random device that will seed the generator
    std::mt19937 lRNG(lSeeder());                             // then make a mersenne twister engine
    const size_t lMin = 0;                                    // random numbers between 0...
    const size_t lMax = Document::getDocumentCount() - 1;     // ...and the current highest doc ID
    std::uniform_int_distribution<size_t> lDistr(lMin, lMax); // the distribution

    const size_t lNumberOfDocuments = DocumentManager::getInstance().getNoDocuments();
    const size_t lNumberOfClusters = std::sqrt(lNumberOfDocuments);
    sizet_vt lIDs; // blacklist for IDs already chosen as leader
    size_t lRandomID;
    for (size_t i = 0; i < lNumberOfClusters; ++i) {
        lRandomID = lDistr(lRNG); // generate random doc id
        auto lDocIt = lDocs.find(lRandomID);
        auto lIDsIt = std::find(lIDs.begin(), lIDs.end(), lRandomID);
        while (lDocIt == lDocs.end() || lIDsIt != lIDs.end()) // doc with ID does not exist or ID in blacklist
        {
            lRandomID = lDistr(lRNG); // generate random doc id
            lDocIt = lDocs.find(lRandomID);
            lIDsIt = std::find(lIDs.begin(), lIDs.end(), lRandomID);
        } // valid doc id found
        _leaders.push_back(&(lDocIt->second));
        lIDs.push_back(lRandomID);
    }
    for (const Document* leaderDocPtr : _leaders) {
        // default constructs the vector for each leader and adds leader to its own cluster
        _cluster[leaderDocPtr].push_back(leaderDocPtr);
    }
    TRACE("Cluster: Leaders chosen.");
}

void Cluster::fillCluster() {
    const doc_mt& lDocs = DocumentManager::getInstance().getDocumentMap();
    for (const auto& doc : lDocs) // doc will we an iterator over the map
    {
        const size_t lIndex = QueryProcessingEngine::getInstance().cosineScoreCluster(&doc.second, _leaders);

        _cluster.at(_leaders[lIndex]).push_back(&doc.second);
    }
    TRACE("Cluster: All documents are assigned to a cluster");
}
