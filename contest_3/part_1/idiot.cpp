#include <iostream>
#include <vector>
#include <algorithm>

struct Command {
  int16_t problems_solved_;
  int32_t penalty_time_;
  size_t index_;
};

bool Compare(const Command& lhs, const Command& rhs) {
  bool result;

  if (lhs.problems_solved_ != rhs.problems_solved_) {
    result = lhs.problems_solved_ > rhs.problems_solved_;
  } else if (lhs.penalty_time_ != rhs.penalty_time_) {
    result = lhs.penalty_time_ < rhs.penalty_time_;
  } else {
    result = lhs.index_ < rhs.index_;
  }

  return result;
}

int main() {
  size_t commands_number;
  Command curr_command;
  std::vector<Command> commands;

  std::cin >> commands_number;

  for (size_t i = 0; i < commands_number; ++i) {
    std::cin >> curr_command.problems_solved_ >> curr_command.penalty_time_;
    curr_command.index_ = i + 1;

    commands.push_back(curr_command);
  }

  std::sort(commands.begin(), commands.end(), Compare);

  for (auto& it : commands) {
    std::cout << it.index_ << std::endl;
  }
}