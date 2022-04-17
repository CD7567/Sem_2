#include <iostream>
#include <string>
#include <unordered_map>
#include <string>

class Radio {
 public:
  Radio() : best_track_(1, 0) {};

  inline bool CanVote(std::string& user_id, uint64_t time) {
    bool result = false;
    auto it_user = times_.find(user_id);

    if (it_user != times_.end()) {
      result = time - it_user->second >= 600;
    } else {
      times_.emplace(user_id, time);
      result = true;
    }

    return result;
  }

  inline void UpdateBest(uint32_t track_id, int64_t score) {
    if (score > best_track_.second || (score == best_track_.second && track_id < best_track_.first)) {
      best_track_.first = track_id;
      best_track_.second = score;
    }
  }

  int64_t Vote(std::string& user_id, uint32_t track_id, int64_t score, uint64_t time) {
    int64_t result;
    auto it_track = tracks_.find(track_id);

    if (CanVote(user_id, time)) {
      if (it_track != tracks_.end()) {
        it_track->second += score;
        result = it_track->second;

        UpdateBest(it_track->first, it_track->second);
      } else {
        tracks_.emplace(track_id, score);
        result = score;

        UpdateBest(track_id, score);
      }
    } else {
      if (it_track != tracks_.end()) {
        result = it_track->second;
      } else {
        tracks_.emplace(track_id, 0);
        result = 0;
      }
    }

    return result;
  }

  std::pair<uint32_t, int64_t> Get() {

    return best_track_;
  }

 private:
  std::unordered_map<std::string, uint64_t> times_;  //  <user_id, last_time>
  std::unordered_map<uint32_t, int64_t> tracks_;  //  <track_id, track_score>

  std::pair<uint32_t, int64_t> best_track_;
};

int main() {
  std::map<std::string, size_t> words;
  std::string word;

  while(std::cin >> word) {
    if (words.count(word) == 0) {
      words.insert(std::make_pair(word, 1));
    } else {
      ++words[word];
    }
  }

  std::vector<std::pair<std::string, size_t>> vect_words(words.begin(), words.end());
  std::sort(vect_words.begin(), vect_words.end(),[](const std::pair<std::string, size_t>& lhs, const std::pair<std::string, size_t>& rhs) {
    bool result;

    if (lhs.second == rhs.second) {
      result = lhs.first < rhs.first;
    } else {
      result = lhs.second > rhs.second;
    }

    return result;
  });

  for (auto& elem : vect_words) {
    std::cout << elem.first << std::endl;
  }
}