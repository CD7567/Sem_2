#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>

#include <big_int.h>
#include <big_int.h>  // check include guards

TEST_CASE("Constructors") {
std::ostringstream oss;

BigInteger a(100050008);
oss << a << '\n';

BigInteger b(-9000000002);
oss << b << '\n';

std::string x_str("1234056789837693278967293875983479857354986798379643835986743598760346745869837498567983769837");
BigInteger x(x_str.c_str());
oss << x << '\n';

std::string y_str("-893749834789698437683498584389573498678943769847398567984327647967984758974398678489509280024");
BigInteger y(y_str.c_str());
oss << y << '\n';

std::string z_str("+102850932486325804128692015804067243109794869810234630820960236842390602398968209386023860120");
BigInteger z(z_str.c_str());
oss << z << '\n';

REQUIRE_FALSE(a.IsNegative());
REQUIRE(b.IsNegative());
REQUIRE_FALSE(x.IsNegative());
REQUIRE(y.IsNegative());
REQUIRE_FALSE(z.IsNegative());

REQUIRE(oss.str() == std::string("100050008\n") + std::string("-9000000002\n") + x_str + "\n" + y_str + "\n" +
z_str.substr(1) + "\n");
}


TEST_CASE("Sum") {
    const std::string large(24, '9');
    const std::string res = "1" + std::string(23, '9') + "8";

    REQUIRE(BigInteger(1234567890) + BigInteger(987654321) == BigInteger("2222222211"));

    REQUIRE(BigInteger(large.c_str()) + BigInteger(large.c_str()) == BigInteger(res.c_str()));

    REQUIRE(-BigInteger(large.c_str()) + -BigInteger(large.c_str()) == -BigInteger(res.c_str()));

    REQUIRE(BigInteger(res.c_str()) + -BigInteger(large.c_str()) == BigInteger(large.c_str()));

    REQUIRE(-BigInteger(res.c_str()) + BigInteger(large.c_str()) == -BigInteger(large.c_str()));
}

TEST_CASE("Subtraction") {
    const std::string large(24, '9');
    const std::string res = "1" + std::string(23, '9') + "8";
    REQUIRE(BigInteger(1234567890) - BigInteger(987654321) == BigInteger("246913569"));
    REQUIRE(BigInteger(res.c_str()) - BigInteger(large.c_str()) == BigInteger(large.c_str()));
    REQUIRE(-BigInteger(res.c_str()) - -BigInteger(large.c_str()) == -BigInteger(large.c_str()));
    REQUIRE(BigInteger(large.c_str()) - -BigInteger(large.c_str()) == BigInteger(res.c_str()));
    REQUIRE(-BigInteger(large.c_str()) - BigInteger(large.c_str()) == -BigInteger(res.c_str()));
}