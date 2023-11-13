#include "primer/trie.h"

#include <stack>
#include <string_view>

#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T* {
    if (this->root_ == nullptr) return nullptr;
    const TrieNode* cnt_node = this->root_.get();
    for (std::size_t i = 0; i < (std::size_t)key.size(); i++) {
        auto itr = cnt_node->children_.find(key[i]);
        if (itr == cnt_node->children_.end()) return nullptr;
        cnt_node = cnt_node->children_.at(key[i]).get();
    }
    if (cnt_node == nullptr) return nullptr;
    if (cnt_node->is_value_node_ == false) return nullptr;
    const TrieNodeWithValue<T>* cnt_value_node =
        dynamic_cast<const TrieNodeWithValue<T>*>(cnt_node);
    if (cnt_value_node == nullptr) return nullptr;
    return cnt_value_node->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
    // Note that `T` might be a non-copyable type. Always use `std::move` when
    // creating `shared_ptr` on that value. throw
    // NotImplementedException("Trie::Put is not implemented.");
    std::shared_ptr<T> shared_value = std::make_shared<T>(std::move(value));
    std::shared_ptr<const TrieNode> cnt_node = this->root_;
    std::stack<std::shared_ptr<const TrieNode>> node_stack;
    for (size_t i = 0; i < (size_t)key.size(); i++) {
        if (cnt_node == nullptr) break;
        node_stack.push(cnt_node);
        if (cnt_node->children_.find(key[i]) == cnt_node->children_.end()) {
            cnt_node = nullptr;
            break;
        } else
            cnt_node = cnt_node->children_.at(key[i]);
    }
    // 注意，如果是原来trie里就有key的话，原叶子结点不能加入到stack里面。
    // 因为不管怎么样叶子结点都不是重复的。
    std::shared_ptr<const TrieNodeWithValue<T>> leaf_node;
    if (cnt_node == nullptr)
        leaf_node = std::make_shared<const TrieNodeWithValue<T>>(
            std::move(shared_value));
    else
        leaf_node = std::make_shared<const TrieNodeWithValue<T>>(
            cnt_node->children_, std::move(shared_value));
    std::shared_ptr<const TrieNode> child_node = leaf_node;
    for (int i = (int)key.size() - 1; i >= (int)node_stack.size(); i--) {
        std::map<char, std::shared_ptr<const TrieNode>> children_map{
            {key[i], child_node}};
        std::shared_ptr<const TrieNode> temp_node =
            std::make_shared<const TrieNode>(children_map);
        child_node = std::move(temp_node);
    }
    for (int i = node_stack.size() - 1; i >= 0 && !node_stack.empty(); i--) {
        std::shared_ptr<const TrieNode> temp_node = node_stack.top();
        node_stack.pop();
        std::shared_ptr<const TrieNode> temp_node_clone = temp_node->Clone();
        const_cast<TrieNode*>(temp_node_clone.get())->children_[key[i]] =
            child_node;
        child_node =
            static_cast<std::shared_ptr<const TrieNode>>(temp_node_clone);
    }
    return Trie(child_node);
    // You should walk through the trie and create new nodes if necessary. If
    // the node corresponding to the key already exists, you should create a new
    // `TrieNodeWithValue`.
}

auto Trie::Remove(std::string_view key) const -> Trie {
    // You should walk through the trie and remove nodes if necessary. If the
    // node doesn't contain a value any more, you should convert it to
    // `TrieNode`. If a node doesn't have children any more, you should remove
    // it.
    std::stack<std::shared_ptr<const TrieNode>> node_stk;
    std::shared_ptr<const TrieNode> cnt_node = root_;
    for (size_t i = 0; i < key.size(); i++) {
        if (cnt_node != nullptr)
            node_stk.push(cnt_node);
        else
            break;
        auto it = cnt_node->children_.find(key[i]);
        if (it == cnt_node->children_.end()) {
            cnt_node = nullptr;
            break;
        } else
            cnt_node = it->second;
    }
    if (cnt_node == nullptr) return *this;
    // cnt_node is the value node now.
    std::shared_ptr<const TrieNode> child_node;
    if (cnt_node->children_.empty())
        child_node = nullptr;
    else
        child_node = std::make_shared<const TrieNode>(cnt_node->children_);
    bool flag = false;  // if real node appeared.
    for (int i = (int)key.size() - 1; i >= 0; i--) {
        if (child_node != nullptr) flag = true;
        cnt_node = node_stk.top();
        node_stk.pop();
        if (flag == false && cnt_node->children_.size() == 1 &&
            cnt_node->is_value_node_ == false) {
            child_node = nullptr;
            cnt_node = nullptr;
            continue;
        }
        cnt_node = cnt_node->Clone();
        const_cast<TrieNode*>(cnt_node.get())->children_.erase(key[i]);
        if (child_node != nullptr)
            const_cast<TrieNode*>(cnt_node.get())->children_[key[i]] =
                child_node;
        child_node = cnt_node;
    }
    return Trie(child_node);
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and
// functions in the header file. However, we separate the implementation into a
// .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate
// them here, so that they can be picked up by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t*;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t*;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string*;

// If your solution cannot compile for non-copy tests, you can remove the below
// lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer*;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked*;

}  // namespace bustub
