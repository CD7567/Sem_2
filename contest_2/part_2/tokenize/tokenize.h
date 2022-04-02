#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

struct PlusToken {
};
struct MinusToken {
};
struct MultiplyToken {
};
struct DivideToken {
};
struct ResidualToken {
};
struct OpeningBracketToken {
};
struct ClosingBracketToken {
};
struct SqrToken {
};
struct MaxToken {
};
struct MinToken {
};
struct AbsToken {
};
struct NumberToken {
  int64_t num_;
};
struct UnknownToken {
  std::string  str_;
};

class Token {
 public:
  std::variant<PlusToken,
               MinusToken,
               MultiplyToken,
               DivideToken,
               ResidualToken,
               OpeningBracketToken,
               ClosingBracketToken,
               SqrToken,
               MaxToken,
               MinToken,
               AbsToken,
               NumberToken,
               UnknownToken> token_;

  bool operator==(const Token&) const;
};

std::vector<Token> Tokenize(std::string_view);