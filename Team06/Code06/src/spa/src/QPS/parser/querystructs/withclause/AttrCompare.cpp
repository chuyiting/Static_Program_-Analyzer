//
// Created by Eddy Chu on 2022/10/2.
//

#include "AttrCompare.h"
#include "QPS/evaluator/CSPUtils.h"
#include "QPS/exceptions/QPSException.h"

#include <memory>
#include <string>

AttrCompare::AttrCompare(std::shared_ptr<WithRef> lhs,
                         std::shared_ptr<WithRef> rhs)
    : lhs(lhs), rhs(rhs) {
  if (lhs->isName() != rhs->isName()) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

bool AttrCompare::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                              assign_t &assign) {
  std::string lhsVal = getWithRefValue(pkb, lhs, assign);
  std::string rhsVal = getWithRefValue(pkb, rhs, assign);
  if (lhsVal == CSPUtils::UNASSIGNED || rhsVal == CSPUtils::UNASSIGNED) {
    return true;
  }

  return lhsVal == rhsVal;
}

std::vector<synonym_t> AttrCompare::getVariables() {
  std::vector<synonym_t> result;
  if (lhs->getRefType() == WithRefType::ATTRREF) {
    auto attrRef = std::dynamic_pointer_cast<AttrWithRef>(lhs);
    result.push_back(attrRef->getSynonym().getSynonymName());
  }

  if (rhs->getRefType() == WithRefType::ATTRREF) {
    auto attrRef = std::dynamic_pointer_cast<AttrWithRef>(rhs);
    result.push_back(attrRef->getSynonym().getSynonymName());
  }

  return result;
}

bool AttrCompare::operator==(const AttrCompare &other) const {
  return *(this->lhs) == *(other.lhs) && *(this->rhs) == *(other.rhs);
}

bool AttrCompare::operator!=(const AttrCompare &other) const {
  return !((*this) == other);
}

std::string AttrCompare::getWithRefValue(std::shared_ptr<PKBGetter> pkb,
                                         std::shared_ptr<WithRef> ref,
                                         assign_t &assign) {
  if (ref->getRefType() == WithRefType::ATTRREF) {
    auto attrRef = std::dynamic_pointer_cast<AttrWithRef>(ref);
    synonym_t synonym = attrRef->getString();

    auto syn = attrRef->getSynonym();
    if (assign[synonym] == CSPUtils::UNASSIGNED) {
      return assign[synonym];
    }

    if ((syn.getSynonymType() == EntityType::CALL &&
         attrRef->getAttrName() == AttributeType::PROCNAME) || // special cases
        (syn.getSynonymType() == EntityType::READ &&
         attrRef->getAttrName() == AttributeType::VARNAME) ||
        (syn.getSynonymType() == EntityType::PRINT &&
         attrRef->getAttrName() == AttributeType::VARNAME)) {
      return pkb->getVarAt(assign[synonym]);
    } else {
      return assign[synonym];
    }
  } else {
    return ref->getString();
  }
}

bool AttrCompare::queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                              orderDomain_t &orderDomain,
                              orderDomain_t &queryResult) {
  if (!hasExactOneVarUnassigned(assign))
    return false;

  std::shared_ptr<AttrWithRef> attrRef;
  std::string val;
  if (lhs->getRefType() == WithRefType::ATTRREF &&
      assign[lhs->getString()] == CSPUtils::UNASSIGNED) {

    attrRef = std::dynamic_pointer_cast<AttrWithRef>(lhs);
    if (rhs->getRefType() == WithRefType::ATTRREF) {
      auto rhsAttr = std::dynamic_pointer_cast<AttrWithRef>(rhs);
      val = rhsAttr->getValue(assign, pkb);
    } else {
      val = rhs->getValue(assign);
    }
  } else {
    attrRef = std::dynamic_pointer_cast<AttrWithRef>(rhs);
    if (lhs->getRefType() == WithRefType::ATTRREF) {
      auto lhsAttr = std::dynamic_pointer_cast<AttrWithRef>(lhs);
      val = lhsAttr->getValue(assign, pkb);
    } else {
      val = lhs->getValue(assign);
    }
  }

  // handle special cases call.PROCNAME, read.VARNAME, print.VARNAME
  if (attrRef->isEntityType(EntityType::CALL) &&
      attrRef->getAttrName() == AttributeType::PROCNAME) {
    queryResult[attrRef->getString()] = pkb->getCallsByCalledProcName(val);
  } else if (attrRef->isEntityType(EntityType::READ) &&
             attrRef->getAttrName() == AttributeType::VARNAME) {
    queryResult[attrRef->getString()] = pkb->getReadsByVarName(val);
  } else if (attrRef->isEntityType(EntityType::PRINT) &&
             attrRef->getAttrName() == AttributeType::VARNAME) {
    queryResult[attrRef->getString()] = pkb->getPrintsByVarName(val);
  } else {
    queryResult[attrRef->getString()] = {val};
  }

  CSPUtils::intersectOrderDomains(queryResult, orderDomain);
  return true;
}

int AttrCompare::priority() { return CSPUtils::PRIORITY_WITH_CLAUSE; }
