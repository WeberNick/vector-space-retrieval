#include "cluster.hh"

/**
 * @brief Construct a new Cluster:: Cluster object
 *
 */
Cluster::Cluster() : 
    _cb(nullptr),
    _leaders(),
    _cluster()
{}

void Cluster::init(const CB& aControlBlock)
{
    if (!_cb)
    {
        _cb = &aControlBlock;
        TRACE("Cluster: Initialized.");
    }
}

void Cluster::chooseLeaders()
{
    const doc_mt& lDocs = DocumentManager::getInstance().getDocumentMap();
    std::random_device lSeeder;                               // the random device that will seed the generator
    std::mt19937 lRNG(lSeeder());                             // then make a mersenne twister engine
    const size_t lMin = 0;                                    // random numbers between 0...
    const size_t lMax = Document::getDocumentCount() - 1;     // ...and the current highest doc ID
    std::uniform_int_distribution<size_t> lDistr(lMin, lMax); // the distribution

    const size_t lNumberOfDocuments = DocumentManager::getInstance().getNoDocuments();
    const size_t lNumberOfClusters = std::sqrt(lNumberOfDocuments);
    sizet_set lUniqueLeader;
    size_t lRandomID;
    while(lUniqueLeader.size() != lNumberOfClusters)
    {
        lRandomID = lDistr(lRNG);                             // generate random doc id
        auto lDocIt = lDocs.find(lRandomID);
        while (lDocIt == lDocs.end())                         // doc with ID does not exist or ID in blacklist
        {
            lRandomID = lDistr(lRNG);                         // generate random doc id
            lDocIt = lDocs.find(lRandomID);
        }                                                     // valid doc id found
        lUniqueLeader.insert((lDocIt->second).getID());       // only inserted if it is no duplicate
    }
    _leaders.assign(lUniqueLeader.begin(), lUniqueLeader.end());
    for (const size_t leaderID : _leaders)
    {
        _cluster[leaderID].push_back(leaderID);               // default constructs the vector for each leader and adds leader to its own cluster
    }
    TRACE("Cluster: Leaders chosen.");
}

void Cluster::getIDs(const std::vector<std::pair<size_t, float>>& aLeaders, const size_t aTopK, sizet_vt& aOutputVec)
{
    for(const auto& leader : aLeaders)
    {
        const sizet_vt& lCluster = _cluster.at(leader.first);
        aOutputVec.insert(aOutputVec.end(), lCluster.begin(), lCluster.end());
        if(aOutputVec.size() >= aTopK)
        {
            break;
        }
    }
}

std::ostream& operator<<(std::ostream& strm, const Cluster& clust) {
    const auto& leaders = clust.getLeaders();
        strm << "Cluster : \n"
            << "Number of Leaders: " << leaders.size() << "\n"
            << "Leader IDs: ";
        for(size_t i = 0; i < leaders.size() - 1; ++i)
        {
            strm << leaders.at(i) << ", ";
        }
        strm << leaders.at(leaders.size() - 1) << std::endl;
    return strm;
}
