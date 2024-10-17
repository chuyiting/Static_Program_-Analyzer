#include "TestWrapper.h"
#include "QPS/QPS.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;

AbstractWrapper *WrapperFactory::createWrapper() {
    if (wrapper == 0) { wrapper = new TestWrapper(); }
    return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() :_pkb(PKB()), _pkb_getter(PKBGetter(_pkb)) , _pkb_setter(PKBSetter(_pkb)),
    qps(QPS(std::make_shared<PKBGetter>(_pkb_getter))) {
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program

}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    // call your parser to do the parsing
    // ...rest of your code...
    SourceProcessor SP = SourceProcessor(std::make_shared<PKBSetter>(_pkb_setter));
    SP.processFile(filename);
    _pkb.done();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...

    _pkb.clearCache();

    auto result = qps.run(query);

    for (const auto& i: result->getResult()) {
        results.push_back(i);
    }

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
}
