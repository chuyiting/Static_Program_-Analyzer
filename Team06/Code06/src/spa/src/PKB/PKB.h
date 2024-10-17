#pragma once

#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <memory>
#include <set>

#include "PKB/type/StmtType.h"
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"

#include "PKB/Datastore/AffectsStorage.h"
#include "PKB/Datastore/CallStorage.h"
#include "PKB/Datastore/FollowStorage.h"
#include "PKB/Datastore/ModifiesStorage.h"
#include "PKB/Datastore/NextStorage.h"
#include "PKB/Datastore/ParentStorage.h"
#include "PKB/Datastore/PatternStorage.h"
#include "PKB/Datastore/SynonymStorage.h"
#include "PKB/Datastore/UsesStorage.h"
#include "PKB/Resolver/EntityResolver/CallsResolver.h"
#include "PKB/Resolver/EntityResolver/CallsTResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/ModifiesPResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/ModifiesSResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/UsesPResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/UsesSResolver.h"
#include "PKB/Resolver/Pattern/AssignPatternResolver.h"
#include "PKB/Resolver/Pattern/IfPatternResolver.h"
#include "PKB/Resolver/Pattern/WhilePatternResolver.h"
#include "PKB/Resolver/StatementResolver/CFG/NextResolver.h"
#include "PKB/Resolver/StatementResolver/CFG/NextTResolver.h"
#include "PKB/Resolver/StatementResolver/CFG/AffectsResolver.h"
#include "PKB/Resolver/StatementResolver/CFG/AffectsTResolver.h"
#include "PKB/Resolver/StatementResolver/Container/FollowResolver.h"
#include "PKB/Resolver/StatementResolver/Container/FollowTResolver.h"
#include "PKB/Resolver/StatementResolver/Container/ParentResolver.h"
#include "PKB/Resolver/StatementResolver/Container/ParentTResolver.h"

using namespace pkb_types;

class PKB {
private:
  /* ================ Storage ================ */
  SynonymStorage _synonym_storage;
  FollowStorage _follows_storage;
  ParentStorage _parent_storage;
  CallStorage _call_storage;
  PatternStorage _pattern_storage;
  ModifiesStorage _modifies_storage;
  UsesStorage _uses_storage;
  NextStorage _next_storage;
  AffectsStorage _affects_storage;

  /* ================ Resolvers ================ */

    FollowResolver followsResolver;
    FollowTResolver followTResolver;
    ParentResolver parentResolver;
    ParentTResolver parentTResolver;
    NextResolver nextResolver;
    NextTResolver nextTResolver;
    AffectsResolver affectsResolver;
    AffectsTResolver affectsTResolver;
    CallsResolver callsResolver;
    CallsTResolver callsTResolver;
    AssignPatternResolver assignPatternResolver;
    IfPatternResolver ifPatternResolver;
    WhilePatternResolver whilePatternResolver;
    ModifiesSResolver modifiesSResolver;
    ModifiesPResolver modifiesPResolver;
    UsesSResolver usesSResolver;
    UsesPResolver usesPResolver;

  friend class PKBSetter;
  friend class PKBGetter;

public:
  PKB();
  ~PKB();

  void done();
  void clearCache();
};

