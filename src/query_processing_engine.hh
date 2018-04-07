
#include <string>
class QueryProcessingEngine {
  public:
    static QueryProcessingEngine& Instance() {
        static QueryProcessingEngine singleton;
        return singleton;
    }

    std::string search(const std::string& string);

  private:
    QueryProcessingEngine() {}
    // ctor hidden
    QueryProcessingEngine(QueryProcessingEngine const&);            // copy ctor hidden
    QueryProcessingEngine& operator=(QueryProcessingEngine const&); // assign op. hidden
    ~QueryProcessingEngine() {}
    // dtor hidden
};