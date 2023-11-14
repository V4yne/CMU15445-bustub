#include <fmt/format.h>
#include <zipfian_int_distribution.h>

#include <bitset>
#include <functional>
#include <numeric>
#include <optional>
#include <random>
#include <thread>  // NOLINT

#include "common/exception.h"
#include "gtest/gtest.h"
#include "primer/trie.h"
#include "primer/trie_answer.h"
#include "trie_debug_answer.h"  // NOLINT

namespace bustub {

TEST(TrieDebugger, TestCase) {
  std::mt19937_64 gen(23333);
  zipfian_int_distribution<uint32_t> dis(0, 1000);

  auto trie = Trie();
  trie = trie.Put<uint32_t>("65", 25);
  trie = trie.Put<uint32_t>("61", 65);
  trie = trie.Put<uint32_t>("82", 84);
  trie = trie.Put<uint32_t>("2", 42);
  trie = trie.Put<uint32_t>("16", 67);
  trie = trie.Put<uint32_t>("94", 53);
  trie = trie.Put<uint32_t>("20", 35);
  trie = trie.Put<uint32_t>("3", 57);
  trie = trie.Put<uint32_t>("93", 30);
  trie = trie.Put<uint32_t>("75", 29);
  // std::cout << "here" << std::endl;
  // std::cout << *trie.Get<uint32_t>("93") << std::endl;
  // auto it = trie.GetRoot()->children_.find('9');
  // auto child = it->second;
  // std::cout << child->children_.size() << std::endl;
  // std::cout << trie.GetRoot()->children_.size() << std::endl;
  // Put a breakpoint here.

  // (1) How many children nodes are there on the root?
  // Replace `CASE_1_YOUR_ANSWER` in `trie_answer.h` with the correct answer.
  if (CASE_1_YOUR_ANSWER != Case1CorrectAnswer()) {
    ASSERT_TRUE(false) << "case 1 not correct";
  }

  // (2) How many children nodes are there on the node of prefix `9`?
  // Replace `CASE_2_YOUR_ANSWER` in `trie_answer.h` with the correct answer.
  if (CASE_2_YOUR_ANSWER != Case2CorrectAnswer()) {
    ASSERT_TRUE(false) << "case 2 not correct";
  }

  // (3) What's the value for `93`?
  // Replace `CASE_3_YOUR_ANSWER` in `trie_answer.h` with the correct answer.
  if (CASE_3_YOUR_ANSWER != Case3CorrectAnswer()) {
    ASSERT_TRUE(false) << "case 3 not correct";
  }
}

}  // namespace bustub
