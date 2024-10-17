#include "PKB.h"

PKB::PKB() :
    _follows_storage(FollowStorage()),
    followsResolver(FollowResolver(_follows_storage)),
    followTResolver(FollowTResolver(_follows_storage)),
    _parent_storage(ParentStorage()),
    parentResolver(ParentResolver(_parent_storage)),
    parentTResolver(ParentTResolver(_parent_storage)),
    _call_storage(CallStorage()),
    callsResolver(CallsResolver(_call_storage)),
    callsTResolver(CallsTResolver(_call_storage)),
    _pattern_storage(PatternStorage()),
    assignPatternResolver(AssignPatternResolver(_pattern_storage)),
    ifPatternResolver(IfPatternResolver(_pattern_storage)),
    whilePatternResolver(WhilePatternResolver(_pattern_storage)),
    _modifies_storage(ModifiesStorage()),
    modifiesSResolver(ModifiesSResolver(_modifies_storage)),
    modifiesPResolver(ModifiesPResolver(_modifies_storage)),
    _uses_storage(UsesStorage()),
    usesSResolver(UsesSResolver(_uses_storage)),
    usesPResolver(UsesPResolver(_uses_storage)),
    _next_storage(NextStorage()),
    nextResolver(NextResolver(_next_storage)),
    nextTResolver(NextTResolver(_next_storage)),
    _affects_storage(AffectsStorage(_next_storage, _modifies_storage, _uses_storage, _synonym_storage)),
    affectsResolver(AffectsResolver(_affects_storage)),
    affectsTResolver(AffectsTResolver(_affects_storage))
{};

PKB::~PKB() = default;

void PKB::done() {
  int totalStmts = 0;
  for (const auto &i : _synonym_storage.getProcedureStatements()) {
    totalStmts += i.second.size();
  }
  _follows_storage.addNode(totalStmts);
  _parent_storage.addNode(totalStmts);
};

void PKB::clearCache() {
  _next_storage.clearCache();
  _affects_storage.clearCache();
};
