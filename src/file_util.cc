#include "file_util.hh"

namespace Util
{
    void readIn(const std::string& aPath, const char aDelimiter, string_vvt& aOutput)
    {
        TRACE(std::string("Start reading the file content at '") + aPath + std::string("'"));
        std::ifstream file(aPath, std::ifstream::in);
        std::string line;
        size_t i = 0;
        while (std::getline(file, line)) {
            Util::splitStringBoost(line, aDelimiter, aOutput[i]);
        }
        file.close();
        TRACE("Finished reading the file content");
    }

    //void readInAndCreateDocs(const std::string& aPath, const char aDelimiter, doc_mt& aDocMap)
    //{
        //string_vvt lFileContent;
        //readIn(aPath, aDelimiter, lFileContent);
        //std::vector<Document> lDocs; //temporarily stores all created documents
        //TRACE("Create all Document objects from the file content and insert into document map");
        //for(const auto& line : lFileContent) //line[0] should be the string ID, line[1] the content
        //{
            //try
            //{
                //string_vt lContent;
                //Util::splitStringBoost(line.at(1), ' ', lContent);
                //const std::string& lDocID = line.at(0);
                //lDocs.emplace_back(lDocID, lContent); //construct all documents in place in this temp vector
            //}
            //catch(const std::out_of_range& ex) 
            //{
                //const std::string lTraceMsg = std::string("The delimiter '") + aDelimiter + std::string("' does not occur in the following line (line will be skipped): \n") + line.at(0); 
                //TRACE(lTraceMsg);
            //}
        //}
        ////move each document from the temp vector to the output map
        //for(const auto& doc : lDocs)
        //{
            //aDocMap.try_emplace(doc.getID(), doc); //inplace copy constructor call
        //}
    //}
}
