#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "SP/SourceProcessor.h"
#include "PKB/PKBSetter.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKB.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include "QPS/tokenizer/QueryTokenizer.h"
#include "QPS/parser/QPSParser.h"
#include "QPS/QPS.h"

class TestWrapper : public AbstractWrapper {
private:
// TODO: remove all pointers -> shared_pointer

// 	- labels: enhancement
    PKB _pkb;
    PKBGetter _pkb_getter;
    PKBSetter _pkb_setter;
    QPS qps;
public:
    // default constructor
    TestWrapper();

    // destructor
    ~TestWrapper();

    // method for parsing the SIMPLE source
    virtual void parse(std::string filename);

    // method for evaluating a query
    virtual void evaluate(std::string query, std::list<std::string> &results);
};

#endif
