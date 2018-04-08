#include "cluster.hh"

Clustering::Clustering() :
    _leaders();
    _cluster();
{
    init();
}

Clustering::~Clustering(){}
    
void Clustering::init()
{
    chooseLeaders();
    initCluster();
    fillCluster();
}

void Clustering::chooseLeaders()
{
    const doc_mt& lDocs = DocumentManager::getInstance().getDocumentMap();
    std::random_device lSeeder; //the random device that will seed the generator
    std::mt19937 lRNG(lSeeder()); //then make a mersenne twister engine
    const size_t lMin = 0; //random numbers between 0...
    const size_t lMax = DocumentManager::getInstance().getCurrID() - 1; //...and the current highest doc ID
    std::uniform_int_distribution<size_t> lDistr(lMin, lMax); //the distribution
    
    const size_t lNumberOfDocuments = DocumentManager::getInstance().getNoDocuments();
    const size_t lNumberOfClusters = std::sqrt(lNumberOfDocuments);
    size_t_vt lIDs; //blacklist for IDs already chosen as leader
    size_t lRandomID;
    for(size_t i = 0; i < lNumberOfClusters; ++i)
    {
        lRandomID = lDistr(lRNG); //generate random doc id
        auto lDocIt = lDocs.find(lRandomID);
        auto lIDsIt = std::find(lIDs.begin(), lIDs.end(), lRandomID);
        while(lDocIt == lDocs.end() || lIDsIt != lIDs.end()) //doc with ID does not exist or ID in blacklist
        {
            lRandomID = lDistr(lRNG); //generate random doc id
            lDocIt = lDocs.find(lRandomID);
            lIDsIt = std::find(lIDs.begin(), lIDs.end(), lRandomID);
        }//valid doc id found
        _leaders.push_back(&(lIterator->second));
        lIDs.push_back(lRandomID);
    }
}

void Cluster::initCluster()
{
    for(const Document* docPtr : _leaders)
    {
        _cluster[docPtr];//default constructs the vector for each leader
    }
}

void Cluster::fillCluster()
{
    const doc_mt& lDocs = DocumentManager::getInstance().getDocumentMap();
    for(const auto& doc : lDocs) //doc will we an iterator over the map
    {
        std::pair<float, const Document*> lSD; //SD = Smallest Distance
        lSD.first = 1; //one is highest dist
        lSD.second = nullptr;
        for(const auto& leader : _leaders)
        {
            float lDist = Util::calcCosDist(*leader, doc.second);
            if(lDist < lSD.first)
            {
                lSD.first = lDist;
                lSD.second = leader;
            }
        }//closest leader found
        _cluster[lSD.second].push_back(&doc.second); //store pointer to doc in the vector
    }
}
