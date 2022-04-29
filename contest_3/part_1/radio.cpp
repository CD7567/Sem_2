#include <iostream>
#include <string>
#include <unordered_map>
#include <set>

using Pair = std::pair<uint64_t, int64_t>;

auto compare = [](const Pair& lhs, const Pair& rhs) {
  return (lhs.second > rhs.second) || (lhs.second == rhs.second && lhs.first < rhs.first);
};

class Radio {
 public:
  Radio() : votes_(compare) {
    for (size_t i = 1; i < 100001; ++i) {
      votes_.emplace(i, 0);
      votes_check_.emplace(i, 0);
    }
  };

  Pair Get() {
    const auto removed = *votes_.begin();
    votes_.erase(votes_.begin());

    votes_.emplace(removed.first, -1);
    votes_check_.find(removed.first)->second = -1;

    return removed;
  }

  int64_t Vote(const std::string& ip, uint64_t track_id, int64_t score, uint64_t time) {
    auto person = people_.find(ip);
    auto track = votes_check_.find(track_id);

    if (person == people_.end()) {
      people_.emplace(ip, time - 600);
      person = people_.find(ip);
    }

    // стопроц валидный итератор на person

    if (time >= person->second + 600) {
      person->second = time;

      if (track == votes_check_.end()) {
        votes_.emplace(track_id, score);
        track = votes_check_.insert({track_id, score}).first;
      } else {
        votes_.erase({track->first, track->second});
        track->second += score;

        votes_.emplace(track->first, track->second);
      }
    } else {
      if (track == votes_check_.end()) {
        votes_.emplace(track_id, 0);
        track = votes_check_.insert({track_id, 0}).first;
      }
    }

    return track->second;
  }

 private:
  std::unordered_map<std::string, uint64_t> people_;
  std::unordered_map<uint64_t, int64_t> votes_check_;
  std::set<Pair, decltype(compare)> votes_;
};

int main() {
  std::string command;
  std::string ip;
  uint64_t track_id, time;
  int64_t score;
  Pair track;
  Radio radio;

  do {
    std::cin >> command;

    if (command == "VOTE") {
      std::cin >> ip >> track_id >> score >> time;

      std::cout << radio.Vote(ip, track_id, score, time) << '\n';
    } else if (command == "GET") {
      track = radio.Get();
      std::cout << track.first << ' ' << track.second << '\n';
    }
  } while (command != "EXIT");

  std::cout << "OK" << '\n';
}