#include <stack>
#include <string_view>

#include "common/exception.h"
#include "primer/trie.h"

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
    if (cnt_node != nullptr) node_stack.push(cnt_node);
    std::cout << key << std::endl;
    std::cout << node_stack.size() << std::endl;
    std::shared_ptr<const TrieNodeWithValue<T>> leaf_node;
    if ((size_t)node_stack.size() != (size_t)key.size() + 1)
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
    std::vector<std::shared_ptr<const TrieNode>>
        node_stack;  // store the same node
    std::shared_ptr<const TrieNode> cur_node = root_;
    std::size_t key_size = key.size();
    decltype(key_size) idx = 0;
    // 1.store the same node
    while (idx < key_size && cur_node) {
        char ch = key[idx++];
        node_stack.push_back(cur_node);
        cur_node = cur_node->children_.find(ch) != cur_node->children_.end()
                       ? cur_node->children_.at(ch)
                       : nullptr;
    }
    if (idx != key_size || !cur_node || !cur_node->is_value_node_) {
        return *this;
    }
    // 2.create end node
    std::shared_ptr<const TrieNode> end_node =
        cur_node->children_.empty()
            ? nullptr
            : std::make_shared<const TrieNode>(cur_node->children_);
    // 3.copy same node
    std::shared_ptr<const TrieNode> child_node = end_node;
    cur_node = end_node;
    for (size_t i = node_stack.size() - 1; i < node_stack.size(); --i) {
        cur_node = std::shared_ptr<const TrieNode>(node_stack[i]->Clone());
        const_cast<TrieNode*>(cur_node.get())->children_[key[i]] = child_node;
        child_node = cur_node;
    }
    return Trie(cur_node);
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

// std::vector<std::shared_ptr<const TrieNode>>
//         node_stack;  // store the same node
//     std::shared_ptr<const TrieNode> cur_node = root_;
//     std::size_t key_size = key.size();
//     decltype(key_size) idx = 0;
//     // 1.store the same node
//     while (idx < key_size && cur_node) {
//         char ch = key[idx++];
//         node_stack.push_back(cur_node);
//         cur_node = cur_node->children_.find(ch) != cur_node->children_.end()
//                        ? cur_node->children_.at(ch)
//                        : nullptr;
//     }
//     if (idx != key_size || !cur_node || !cur_node->is_value_node_) {
//         return *this;
//     }
//     // 2.create end node
//     std::shared_ptr<const TrieNode> end_node =
//         cur_node->children_.empty()
//             ? nullptr
//             : std::make_shared<const TrieNode>(cur_node->children_);
//     // 3.copy same node
//     std::shared_ptr<const TrieNode> child_node = end_node;
//     cur_node = end_node;
//     bool flag = false;
//     for (size_t i = node_stack.size() - 1; i < node_stack.size(); --i) {
//         if (cur_node != nullptr) flag = true;
//         cur_node = std::shared_ptr<const TrieNode>(node_stack[i]->Clone());
//         if (cur_node->children_.size() == 1 && flag == false) {
//             cur_node = nullptr;
//         } else {
//             const_cast<TrieNode*>(cur_node.get())->children_[key[i]] =
//                 child_node;
//         }
//         child_node = cur_node;
//     }
//     return Trie(cur_node);