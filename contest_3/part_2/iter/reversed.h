#pragma once

#define REVERSE_REVERSED_IMPLEMENTED

#include <cstdint>
#include <algorithm>

template <typename Collection,
          typename CollIterator = std::conditional_t<std::is_const_v<Collection>, typename Collection::const_iterator,
                                                     typename Collection::iterator>>
class Reversed {
 public:
  using value_type = std::conditional_t<std::is_const_v<Collection>, const typename Collection::value_type,  //  NOLINT
                                        typename Collection::value_type>;                                    //  NOLINT
  using reference = value_type&;                                                                             //  NOLINT
  using const_reference = const value_type&;                                                                 //  NOLINT

  using iterator = std::reverse_iterator<CollIterator>;                               //  NOLINT
  using const_iterator = std::reverse_iterator<typename Collection::const_iterator>;  //  NOLINT
  using reversed_iterator = CollIterator;                                             //  NOLINT
  using const_reversed_iterator = typename Collection::const_iterator;                //  NOLINT

  Reversed() = delete;
  explicit Reversed(Collection& coll) : temp_collection_(), begin_(coll.begin()), end_(coll.end()) {
  }
  explicit Reversed(Collection&& coll)
      : temp_collection_(coll), begin_(temp_collection_.begin()), end_(temp_collection_.end()) {
  }
  Reversed(Reversed&) = delete;
  Reversed(Reversed&&) = delete;

  ~Reversed() = default;

  iterator begin() const {  //  NOLINT
    return end_;
  }

  iterator end() const {  //  NOLINT
    return begin_;
  }

  reversed_iterator rbegin() const {  //  NOLINT
    return begin_.base();
  }

  reversed_iterator rend() const {  //  NOLINT
    return end_.base();
  }

 private:
  Collection temp_collection_;
  iterator begin_;
  iterator end_;
};