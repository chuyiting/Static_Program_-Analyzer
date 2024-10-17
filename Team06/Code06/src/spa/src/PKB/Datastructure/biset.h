//
// Created by Stephen Tan  Hin Khai on 24/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_BISET_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_BISET_H_

#include "utils/map_utils.h"
#include <map>
#include <set>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "utils/map_utils.h"

namespace biset {
    /**
     * Biset such that:
     * key -> ValueSet
     * value -> KeySet
     * @tparam _KeyType
     * @tparam _MappedType
     * @tparam _KeyContainer
     * @tparam _MappedContainer
     * @tparam _KeyValue
     * @tparam _MappedValue
     */
    template < typename _KeyType,
	       typename _MappedType,
	       typename _KeyValue = std::set<_MappedType>,
	       typename _MappedValue = std::set<_KeyType>,
	       typename _KeyContainer = std::map<_KeyType, _MappedValue>,
	       typename _MappedContainer = std::map<_MappedType, _KeyValue>>
      class biset {

	  using key_type = _KeyType;
	  using value_type = _KeyValue;
	  using rkey_type = _MappedType;
	  using rvalue_type = _MappedValue;

	private:
	  using _Tree = _KeyContainer;
	  using _Tree_Node = std::pair<key_type, value_type>;
	  // reversed map
	  using _RTree = _MappedContainer;
	  using _RTree_Node = std::pair<rkey_type, rvalue_type>;

	private:
	  // static assertion
	  static_assert(std::is_same_v<key_type, typename _Tree::key_type>,
			"biset::key_type must match _Container_Type::key_type");
	  static_assert(
	      std::is_same_v<rkey_type, typename _RTree::key_type>,
	      "biset::value_type must match _Reversed_Container_Type::key_type");

	  static_assert(
	      std::is_same_v<_KeyValue, typename _Tree::mapped_type>,
	      "biset::_KeyValue must match _Container::value_type");

	  static_assert(
	      std::is_same_v<_MappedValue, typename _RTree::mapped_type>,
	      "biset::_MappedValue must match _Reversed_Container::value_type");

	private:
	  _Tree m_key_tree;
	  _RTree m_value_tree;

	public:
	  /* Constructor */
	  biset() = default;

	  biset(std::initializer_list<_Tree_Node> init) {
	    for (const auto &elem : init) {
	      insert(elem);
	    }
	  }

	  void insert(_Tree_Node &&node) {
	    m_key_tree.insert(node);
	    for (auto i : node.second) {
	      m_value_tree[i].insert(node.first);
	    }
	  }

	  void insert(const _Tree_Node &node) {
	    m_key_tree.insert(node);
	    for (auto i : node.second) {
	      m_value_tree[i].insert(node.first);
	    }
	  }

	  void insert(const key_type &key, const value_type &value) {
	    m_key_tree[key].insert(value.begin(), value.end());
	    for (auto i : value) {
	      m_value_tree[i].insert(key);
	    }
	  }

	  void erase_key(const key_type &key) {
	    auto k_iter = m_value_tree.find(key);
	    for (auto i : k_iter->second) {
	      auto v_iter = m_key_tree.find(i);
	      v_iter->second.erase(key);
	    }
	    m_key_tree.erase(k_iter);
	  }

	  void erase_value(const value_type &value) {
	    auto v_iter = m_value_tree.find(value);
	    for (auto i : v_iter->second) {
	      auto k_iter = m_key_tree.find(i);
	      k_iter->second.erase(value);
	    }
	    m_value_tree.erase(v_iter);
	  }

	public:
	  bool has_key(const key_type &key) const {
	    return m_key_tree.find(key) != m_key_tree.end() && !m_key_tree.find(key)->second.empty();
	  }

	  bool has_value(const rkey_type &value) const {
	    return m_value_tree.find(value) != m_value_tree.end() && !m_value_tree.find(value)->second.empty();
	  }

	  // checks that key exists, and that it contains the value
	  bool has_keymapping(const key_type &key, const rkey_type &value) const {
	    return m_key_tree.find(key) != m_key_tree.end()
		&& map_utils::isIterableContains(m_key_tree.find(key)->second, value);
	  }

	  // checks that value exists, and that it contains the key
	  bool has_valuemapping(const rkey_type &value, const key_type &key) const {
	    return m_value_tree.find(value) != m_value_tree.end()
	    && map_utils::isIterableContains(m_value_tree.find(value)->second, key);
	  }

	  value_type get_valueset(const key_type &key) const {
	    auto v_iter = m_key_tree.find(key);
	    if (map_utils::isIterableContains(m_key_tree, key)) {
	      return v_iter->second;
	    } else {
	      return value_type({});
	    }
	  }

	  rvalue_type get_keyset(const rkey_type &value) const {
	    auto k_iter = m_value_tree.find(value);
	    if (map_utils::isIterableContains(m_value_tree, value)) {
	      return k_iter->second;
	     } else {
	      return rvalue_type({});
	    }

	  }

	  _Tree getAllKeyMapping() {
	    return m_key_tree;
	  }

	  _Tree getAllValueMapping() {
	    return m_value_tree;
	  }

	  size_t size() const { return m_key_tree.size(); }
      };

    template < class _KeyType, class _MappedType >
      using unordered_biset =
	  biset<_KeyType, _MappedType, std::unordered_set<_MappedType>, std::unordered_set<_KeyType>,
	  	std::unordered_map<_KeyType, std::unordered_set<_MappedType>>,
		std::unordered_map<_MappedType, std::unordered_set<_KeyType>>
	  >;

} // namespace bimap

#endif //SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_BISET_H_
