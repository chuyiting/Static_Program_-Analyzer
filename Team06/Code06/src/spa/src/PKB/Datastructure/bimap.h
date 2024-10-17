// #pragma clang diagnostic push
// #pragma ide diagnostic ignored "bugprone-reserved-identifier"
//
//  Created by Stephen Tan  Hin Khai on 7/9/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORAGE_BIMAP_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORAGE_BIMAP_H_
#endif

#include <map>
#include <unordered_map>

namespace bimap {
// Todo make it work with STL for personal learning purposes.
template <typename _KeyType, typename _ValueType,
          typename _KeyContainer = std::map<_KeyType, _ValueType>,
          typename _MappedContainer = std::map<_ValueType, _KeyType>>
class bimap {

  using key_type = _KeyType;
  using value_type = _ValueType;
  using rkey_type = _ValueType;
  using rvalue_type = _KeyType;

private:
  using _Tree = _KeyContainer;
  using _Tree_Node = std::pair<key_type, value_type>;
  // reversed map
  using _RTree = _MappedContainer;
  using _RTree_Node = std::pair<rkey_type, rvalue_type>;

private:
  // static assertion
  static_assert(std::is_same_v<key_type, typename _Tree::key_type>,
                "bimap::key_type must match _Container_Type::key_type");
  static_assert(
      std::is_same_v<value_type, typename _RTree::key_type>,
      "bimap::value_type must match _Reversed_Container_Type::value_type");

private:
  _Tree m_key_tree;
  _RTree m_value_tree;

public:
  /* Constructor */
  bimap() = default;

  bimap(std::initializer_list<_Tree_Node> init) {
    for (const auto &elem : init) {
      insert(elem);
    }
  }

  void insert(_Tree_Node &&node) {
    m_key_tree.insert(node);
    m_value_tree.insert({node.second, node.first});
  }

  void insert(const _Tree_Node &node) {
    m_key_tree.insert(node);
    m_value_tree.insert({node.second, node.first});
  }

  void insert(const key_type &key, const value_type &value) {
    m_key_tree.insert({key, value});
    m_value_tree.insert({value, key});
  }

  void erase_key(const key_type &key) {
    auto k_iter = m_value_tree.find(key);
    auto v_iter = m_key_tree.find(k_iter->second);
    m_key_tree.erase(k_iter);
    m_value_tree.erase(v_iter);
  }

  void erase_value(const value_type &value) {
    auto v_iter = m_key_tree.find(value);
    auto k_iter = m_value_tree.find(v_iter->second);
    m_key_tree.erase(k_iter);
    m_value_tree.erase(v_iter);
  }

public:
  bool has_key(const key_type &key) const {
    return m_key_tree.find(key) != m_key_tree.end();
  }

  bool has_value(const value_type &value) const {
    return m_value_tree.find(value) != m_value_tree.end();
  }

  value_type get_value(const key_type &key) const {
    auto v_iter = m_key_tree.find(key);
    return v_iter->second;
  }

  key_type get_key(const value_type &value) const {
    auto k_iter = m_value_tree.find(value);
    return k_iter->second;
  }

  size_t size() const { return m_key_tree.size(); }
};

template <class _KeyType, class _ValueType>
using unordered_bimap =
    bimap<_KeyType, _ValueType, std::unordered_map<_KeyType, _ValueType>,
          std::unordered_map<_ValueType, _KeyType>>;

} // namespace bimap
