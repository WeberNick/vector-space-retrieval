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
    if (!_init) {
        _cb = &aControlBlock;
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
    size_t lRandomID;
    while(_leaders.size() != lNumberOfClusters) 
    {
        lRandomID = lDistr(lRNG); // generate random doc id
        auto lDocIt = lDocs.find(lRandomID);
        while(lDocIt == lDocs.end()) // doc with ID does not exist or ID in blacklist
        {
            lRandomID = lDistr(lRNG); // generate random doc id
            lDocIt = lDocs.find(lRandomID);
        } // valid doc id found
        _leaders.insert((lDocIt->second).getID()); //only inserted if it is no duplicate
    }
    _leadersVec = sizet_vt(_leaders.begin(), _leaders.end());
    for (const size_t leaderID : _leaders) {
        // default constructs the vector for each leader and adds leader to its own cluster
        _cluster[leaderID].push_back(leaderID);
    }
    TRACE("Cluster: Leaders chosen.");
}

const sizet_vt Cluster::getIDs(const std::vector<std::pair<size_t, float>>& aLeaders, const size_t aTopK)
{
    size_t lIDsCount = 0;
    sizet_vt lOutput;
    for(const auto& leader : aLeaders)
    {
        const sizet_vt& lCluster = _cluster.at(leader.first);
        lOutput.insert(lOutput.end(), lCluster.begin(), lCluster.end());
        if(lOutput.size() >= aTopK)
        {
            return lOutput;
        }
    }
    return lOutput;
}
