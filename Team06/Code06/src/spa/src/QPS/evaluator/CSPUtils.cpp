//
// Created by Eddy Chu on 2022/10/15.
//

#include "CSPUtils.h"

void CSPUtils::differenceOrderDomainWithInference(
    orderDomain_t &orderDomain, const inference_t &inference) {
  for (auto &[syn, infer] : inference) {
    for (auto &val : infer) {
      orderDomain[syn].insert(val);
    }
  }
}

bool CSPUtils::isAllVariableAssigned(assign_t &assign) {
  for (auto const &[_, val] : assign) {
    if (val == UNASSIGNED) {
      return false;
    }
  }

  return true;
}

void CSPUtils::computeDomain(const synonym_t &target, domain_t &domain,
                             orderDomain_t &orderDomain,
                             inference_t &inference) {
  domain_t &original = orderDomain.find(target)->second;
  domain_t &infer = inference.find(target)->second;

  std::copy_if(original.begin(), original.end(),
               std::inserter(domain, domain.begin()),
               [&infer](const synonym_t &syn) {
                 return infer.find(syn) == infer.end();
               });
}

void CSPUtils::unionOrderDomainWithInference(orderDomain_t &orderDomain,
                                             const inference_t &inference) {
  for (auto &[syn, infer] : inference) {
    domain_t result;
    std::copy_if(orderDomain[syn].begin(), orderDomain[syn].end(),
                 std::inserter(result, result.begin()),
                 [&infer = infer](const synonym_t &synonym) {
                   return infer.find(synonym) == infer.end();
                 });

    orderDomain[syn] = result; // there is a copy happening here
  }
}

/**
 * Only intersectOrderDomains the sets that exist in orderDomain2, the rest will
 * remain intact in orderDomain1, The sets that only appear in orderDomain2 will
 * not be added to orderDomain 1 In place intersection to orderDomain1
 * @param orderDomain1
 * @param orderDomain2
 */
void CSPUtils::intersectOrderDomains(orderDomain_t &orderDomain1,
                                     orderDomain_t &orderDomain2) {
  orderDomain_t &majorOrderDomain =
      orderDomain1.size() > orderDomain2.size() ? orderDomain1 : orderDomain2;
  orderDomain_t &minorOrderDomain =
      orderDomain1.size() <= orderDomain2.size() ? orderDomain1 : orderDomain2;

  for (auto &[syn, domain] : minorOrderDomain) {
    if (majorOrderDomain.find(syn) == majorOrderDomain.end())
      continue;

    domain_t result;
    domain_t &minorDomain = domain.size() < majorOrderDomain[syn].size()
                                ? domain
                                : majorOrderDomain[syn];
    domain_t &majorDomain = domain.size() >= majorOrderDomain[syn].size()
                                ? domain
                                : majorOrderDomain[syn];

    std::copy_if(minorDomain.begin(), minorDomain.end(),
                 std::inserter(result, result.begin()),
                 [&d = majorDomain](const std::string &val) {
                   return d.find(val) != d.end();
                 });

    orderDomain1[syn] = result; // there is a copy happening here
  }
}

void CSPUtils::differenceDomains(domain_t &domain1,
                                      domain_t &domain2) {
  domain_t result;
  std::copy_if(domain1.begin(), domain1.end(),
               std::inserter(result, result.begin()),
               [&d = domain2](const std::string &val) {
                 return d.find(val) == d.end();
               });
  domain1 = result;
}

void CSPUtils::initInference(inference_t &inference, assign_t &assign) {
  for (auto const &[synonym, _] : assign) {
    domain_t emptySet;
    inference.insert(std::make_pair(synonym, emptySet));
  }
}
