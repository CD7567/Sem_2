#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include "rational.h"
#include "rational.h"  // check include guards

void RationalEqual(const Rational& rational, int64_t numerator, int64_t denominator) {
    REQUIRE(rational.GetNumerator() == numerator);
    REQUIRE(rational.GetDenominator() == denominator);
}

TEST_CASE("Constructors") {
    Rational a;
    // Check default constructor
    RationalEqual(a, 0, 1);

    Rational b = -1;
    // Check constructor from int
    RationalEqual(b, -1, 1);
    b = -1l;
    RationalEqual(b, -1, 1);
    b = -1ll;
    RationalEqual(b, -1, 1);

    // Check simplifying
    Rational c(2, -4);
    RationalEqual(c, -1, 2);

    Rational d(0, 5);
    RationalEqual(d, 0, 1);
    d = {0, -5};
    RationalEqual(d, 0, 1);

    REQUIRE_THROWS_AS(Rational(1, 0), RationalDivisionByZero);
}

TEST_CASE("Getters & Setters") {
    // Not checking getters

    Rational a;
    // Check simplifying
    a.SetNumerator(6);
    a.SetDenominator(-21);
    RationalEqual(a, -2, 7);

    a.SetNumerator(0);
    RationalEqual(a, 0, 1);

    REQUIRE_THROWS_AS(a.SetDenominator(0), RationalDivisionByZero);
}

TEST_CASE("Unary operators") {
    Rational a(1, 2);
    Rational b(0, 10);

    RationalEqual(+a, 1, 2);
    RationalEqual(+b, 0, 1);
    RationalEqual(-a, -1, 2);
    RationalEqual(-b, 0, 1);
}

TEST_CASE("Binary") {
    Rational a(1, 2);
    Rational b(-3,7);
    Rational c(0, 4);
    Rational d(9, 8);
    Rational e(5, 2);
    Rational f(-5,2);
    Rational m(INT32_MAX, 2);

    RationalEqual(m * (m / m), INT32_MAX, 2);
    RationalEqual(m + m, INT32_MAX, 1);

    RationalEqual(a + c, 1, 2);
    RationalEqual(a + b, 1, 14);
    RationalEqual(e + f, 0, 1);
    RationalEqual(e - f, 5, 1);
    RationalEqual(a - b, 13, 14);
    RationalEqual(a - c, 1, 2);
    RationalEqual(b * c, 0, 1);
    RationalEqual(a * b, -3, 14);
    RationalEqual(c / a, 0, 1);
    RationalEqual(b / a, -6, 7);
    RationalEqual(a / b, -7, 6);
    REQUIRE_THROWS_AS(a / c, RationalDivisionByZero);

    a += b;
    RationalEqual(a, 1, 14);
    b -= a;
    RationalEqual(b, -1, 2);
    c *= a;
    RationalEqual(c, 0, 1);
    a *= c;
    RationalEqual(a, 0, 1);
    a /= b;
    RationalEqual(a, 0, 1);
    b /= d;
    RationalEqual(b, -4, 9);
    REQUIRE_THROWS_AS(b /= a, RationalDivisionByZero);
}

TEST_CASE("Increments") {
    Rational a;
    Rational b = -1;
    Rational c(2, -4);

    RationalEqual(++a, 1, 1);
    RationalEqual(a++, 1, 1);
    RationalEqual(a, 2, 1);
    RationalEqual(++c, 1, 2);

    RationalEqual(--b, -2, 1);
    RationalEqual(b--, -2, 1);
    RationalEqual(b--, -3, 1);
    RationalEqual(--c, -1, 2);
}

TEST_CASE("Compare") {
    Rational a;
    Rational b = -1;
    Rational c(2, -4);

    REQUIRE(a > b);
    REQUIRE(c > b);
    REQUIRE(a >= b);
    REQUIRE(a >= a);
    REQUIRE(c >= b);
    REQUIRE(b < c);
    REQUIRE(b <= c);
    REQUIRE(a != c);
    REQUIRE_FALSE(a != a);
    REQUIRE_FALSE(a == c);
    REQUIRE(a == a);
}

TEST_CASE("Streams") {
    Rational a;
    std::string max_int = std::to_string(INT32_MAX);
    std::string max_neg_int = std::to_string(-INT32_MAX);

    std::stringstream is("10 1/0 10/-7 10/-15 " + max_int + '/' + max_neg_int + ' ' + max_neg_int + '/' + max_neg_int + ' ' + max_neg_int + ' ' + max_neg_int + "/1 -0/5");
    std::stringstream os;

    is >> a;
    RationalEqual(a, 10, 1);
    os << a << ' ';

    REQUIRE_THROWS_AS(is >> a, RationalDivisionByZero);

    is >> a;
    RationalEqual(a, -10, 7);
    os << a << ' ';

    is >> a;
    RationalEqual(a, -2, 3);
    os << a << ' ';

    is >> a;
    RationalEqual(a, -1, 1);
    os << a << ' ';

    is >> a;
    RationalEqual(a, 1, 1);
    os << a << ' ';

    is >> a;
    RationalEqual(a, -INT32_MAX, 1);
    os << a << ' ';

    is >> a;
    RationalEqual(a, -INT32_MAX, 1);
    os << a << ' ';

    is >> a;
    RationalEqual(a, 0, 1);
    os << a;

    REQUIRE(os.str() == "10 -10/7 -2/3 -1 1 " + max_neg_int + ' ' + max_neg_int + " 0");
}

TEST_CASE("Exception") {
    Rational a(1);

    REQUIRE_THROWS_AS((void)(a / 0), RationalDivisionByZero);  // NOLINT
}