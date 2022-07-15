//===---------------------------------------------------------------------===//
// statistics_test - Unit tests for src/statistics.cc
//===---------------------------------------------------------------------===//

#include "../src/internal_macros.h"
#include "../src/string_util.h"
#include "gtest/gtest.h"

namespace {
TEST(StringUtilTest, stoul) {
  {
    size_t pos = 0;
    EXPECT_EQ(0ul, benchmark::stoul("0", &pos));
    EXPECT_EQ(1ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(7ul, benchmark::stoul("7", &pos));
    EXPECT_EQ(1ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(135ul, benchmark::stoul("135", &pos));
    EXPECT_EQ(3ul, pos);
  }
#if ULONG_MAX == 0xFFFFFFFFul
  {
    size_t pos = 0;
    EXPECT_EQ(0xFFFFFFFFul, benchmark::stoul("4294967295", &pos));
    EXPECT_EQ(10ul, pos);
  }
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFFul
  {
    size_t pos = 0;
    EXPECT_EQ(0xFFFFFFFFFFFFFFFFul,
              benchmark::stoul("18446744073709551615", &pos));
    EXPECT_EQ(20ul, pos);
  }
#endif
  {
    size_t pos = 0;
    EXPECT_EQ(10ul, benchmark::stoul("1010", &pos, 2));
    EXPECT_EQ(4ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(520ul, benchmark::stoul("1010", &pos, 8));
    EXPECT_EQ(4ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(1010ul, benchmark::stoul("1010", &pos, 10));
    EXPECT_EQ(4ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(4112ul, benchmark::stoul("1010", &pos, 16));
    EXPECT_EQ(4ul, pos);
  }
  {
    size_t pos = 0;
    EXPECT_EQ(0xBEEFul, benchmark::stoul("BEEF", &pos, 16));
    EXPECT_EQ(4ul, pos);
  }
#ifndef BENCHMARK_HAS_NO_EXCEPTIONS
  { ASSERT_THROW(benchmark::stoul("this is a test"), std::invalid_argument); }
#endif
}

TEST(StringUtilTest, stoi){{size_t pos = 0;
EXPECT_EQ(0, benchmark::stoi("0", &pos));
EXPECT_EQ(1ul, pos);
}  // namespace
{
  size_t pos = 0;
  EXPECT_EQ(-17, benchmark::stoi("-17", &pos));
  EXPECT_EQ(3ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(1357, benchmark::stoi("1357", &pos));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(10, benchmark::stoi("1010", &pos, 2));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(520, benchmark::stoi("1010", &pos, 8));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(1010, benchmark::stoi("1010", &pos, 10));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(4112, benchmark::stoi("1010", &pos, 16));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(0xBEEF, benchmark::stoi("BEEF", &pos, 16));
  EXPECT_EQ(4ul, pos);
}
#ifndef BENCHMARK_HAS_NO_EXCEPTIONS
{ ASSERT_THROW(benchmark::stoi("this is a test"), std::invalid_argument); }
#endif
}

TEST(StringUtilTest, stod){{size_t pos = 0;
EXPECT_EQ(0.0, benchmark::stod("0", &pos));
EXPECT_EQ(1ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(-84.0, benchmark::stod("-84", &pos));
  EXPECT_EQ(3ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(1234.0, benchmark::stod("1234", &pos));
  EXPECT_EQ(4ul, pos);
}
{
  size_t pos = 0;
  EXPECT_EQ(1.5, benchmark::stod("1.5", &pos));
  EXPECT_EQ(3ul, pos);
}
{
  size_t pos = 0;
  /* Note: exactly representable as double */
  EXPECT_EQ(-1.25e+9, benchmark::stod("-1.25e+9", &pos));
  EXPECT_EQ(8ul, pos);
}
#ifndef BENCHMARK_HAS_NO_EXCEPTIONS
{ ASSERT_THROW(benchmark::stod("this is a test"), std::invalid_argument); }
#endif
}

TEST(StringUtilTest, StrSplit) {
  EXPECT_EQ(benchmark::StrSplit("", ','), std::vector<std::string>{});
  EXPECT_EQ(benchmark::StrSplit("hello", ','),
            std::vector<std::string>({"hello"}));
  EXPECT_EQ(benchmark::StrSplit("hello,there,is,more", ','),
            std::vector<std::string>({"hello", "there", "is", "more"}));
}

// Wrap benchmark::AppendHumanReadable to use a return value instead.
std::string HumanReadableIec(int n) {
  std::string result{" "};
  benchmark::AppendHumanReadable(n, &result);
  return result;
}

TEST(StringUtilTest, AppendHumanReadable) {
  EXPECT_EQ(HumanReadableIec(0), " 0");
  EXPECT_EQ(HumanReadableIec(1024 - 1), " 1023");
  EXPECT_EQ(HumanReadableIec(1024), " 1Ki");
  EXPECT_EQ(HumanReadableIec(1024 + 1), " 1Ki");
  EXPECT_EQ(HumanReadableIec(1024 * 1024 - 1), " 1024Ki");
  EXPECT_EQ(HumanReadableIec(1024 * 1024), " 1Mi");
  EXPECT_EQ(HumanReadableIec(1024 * 1024 + 1), " 1Mi");

  EXPECT_EQ(HumanReadableIec(-1024), " -1Ki");
}

TEST(StringUtilTest, HumanReadableNumber) {
  const double kIs1000 = 1000;

  // Integers without an SI suffix.
  EXPECT_EQ(benchmark::HumanReadableNumber(0, kIs1000), "0");
  EXPECT_EQ(benchmark::HumanReadableNumber(1, kIs1000), "1");
  EXPECT_EQ(benchmark::HumanReadableNumber(-1, kIs1000), "-1");
  EXPECT_EQ(benchmark::HumanReadableNumber(11, kIs1000), "11");
  EXPECT_EQ(benchmark::HumanReadableNumber(101, kIs1000), "101");
  EXPECT_EQ(benchmark::HumanReadableNumber(1001, kIs1000), "1001");

  // Doubles >1 without an SI suffix.
  // In case of 1 < n < 10, displays a third sig fig.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.044, kIs1000), "1.04");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.046, kIs1000), "1.05");
  // Otherwise, just displays precision == 1 to indicate roundoff.
  EXPECT_EQ(benchmark::HumanReadableNumber(10.44, kIs1000), "10.4");
  EXPECT_EQ(benchmark::HumanReadableNumber(10.46, kIs1000), "10.5");
  EXPECT_EQ(benchmark::HumanReadableNumber(100.44, kIs1000), "100.4");
  EXPECT_EQ(benchmark::HumanReadableNumber(100.46, kIs1000), "100.5");
  EXPECT_EQ(benchmark::HumanReadableNumber(1000.44, kIs1000), "1000.4");
  EXPECT_EQ(benchmark::HumanReadableNumber(1000.46, kIs1000), "1000.5");

  // Doubles <1 without an SI suffix.  Displays two sig figs.
  EXPECT_EQ(benchmark::HumanReadableNumber(0.144, kIs1000), "0.14");
  EXPECT_EQ(benchmark::HumanReadableNumber(0.146, kIs1000), "0.15");
  EXPECT_EQ(benchmark::HumanReadableNumber(0.0144, kIs1000), "0.014");
  EXPECT_EQ(benchmark::HumanReadableNumber(0.0146, kIs1000), "0.015");

  // Numbers >1 with SI suffix and above the 10% margin.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e3, kIs1000), "1.2k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e4, kIs1000), "12.0k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e5, kIs1000), "120.0k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e6, kIs1000), "1.2M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e7, kIs1000), "12.0M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e8, kIs1000), "120.0M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e9, kIs1000), "1.2G");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e10, kIs1000), "12.0G");

  // Numbers >1 with SI suffix and within the 0..10% margin.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e4, kIs1000), "10.4k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e5, kIs1000), "104.0k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e6, kIs1000), "1040.0k");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e7, kIs1000), "10.4M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e8, kIs1000), "104.0M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e9, kIs1000), "1040.0M");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e10, kIs1000), "10.4G");

  // Numbers <1 with SI suffix and above the 10% margin.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-3, kIs1000), "1.2m");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-4, kIs1000), "120.0u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-5, kIs1000), "12.0u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-6, kIs1000), "1.2u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-7, kIs1000), "120.0n");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-8, kIs1000), "12.0n");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.2e-9, kIs1000), "1.2n");

  // Numbers <1 with SI suffix and within the 0..10% margin.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-3, kIs1000), "1040.0u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-4, kIs1000), "104.0u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-5, kIs1000), "10.4u");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-6, kIs1000), "1040.0n");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-7, kIs1000), "104.0n");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-8, kIs1000), "10.4n");
  EXPECT_EQ(benchmark::HumanReadableNumber(1.04e-9, kIs1000), "1040.0p");

  // Stupendously tiny and huge doubles.
  EXPECT_EQ(benchmark::HumanReadableNumber(1.45e100, kIs1000), "1.5e+100");
  EXPECT_EQ(benchmark::HumanReadableNumber(-1e-200, kIs1000), "-1.0e-200");
}

}  // end namespace
