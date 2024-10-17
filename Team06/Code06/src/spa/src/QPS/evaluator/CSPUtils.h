//
// Created by Eddy Chu on 2022/10/15.
//

#ifndef SPA_CSPUTILS_H
#define SPA_CSPUTILS_H

#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPSConstraint.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_map<synonym_t,
                           std::vector<std::shared_ptr<QPSConstraint>>>
    constraints_t;
typedef std::unordered_map<synonym_t, domain_t> inference_t;

class CSPUtils {
public:
  inline static const std::string UNASSIGNED =
      "0%$!"; // some value that breaks semantic rules
  inline static const std::string ASSIGNED =
      "0!Assigned%$!"; // some value that breaks semantic rules

  inline static const int PRIORITY_WITH_CLAUSE = 10;
  inline static const int PRIORITY_PATTERN_CLAUSE = 5;
  inline static const int PRIORITY_THRESHOLD = 10;

  static void unionOrderDomainWithInference(orderDomain_t &orderDomain,
                                            const inference_t &inference);

  static void intersectOrderDomains(orderDomain_t &orderDomain1,
                                 orderDomain_t &orderDomain2);

  static void differenceOrderDomainWithInference(orderDomain_t &orderDomain,
                                                 const inference_t &inference);

  static void differenceDomains(domain_t &domain1,
                                     domain_t &domain2);

  static bool isAllVariableAssigned(assign_t &assign);

  static void computeDomain(const synonym_t &target, domain_t &domain,
                            orderDomain_t &orderDomain, inference_t &inference);

  static void initInference(inference_t &inference, assign_t &assign);
};

#endif // SPA_CSPUTILS_H
