#ifndef BENCHMARK_STRING_UTIL_H_
#define BENCHMARK_STRING_UTIL_H_

#include <sstream>
#include <string>
#include <utility>

#include "benchmark/export.h"
#include "internal_macros.h"

namespace benchmark {

void AppendHumanReadable(int n, std::string* str);

// HumanReadableNumber formats `n` using an SI or IEC suffix and rounding.
// There is a 10% margin such that, e.g., 1040 is shown as "1040" not "1.0k".
//
// The `one_k` determines whether we are using SI (e.g., kilobytes "k") or IEC
// (e.g., kibibytes "Ki") per the benchmark::Counter::OneK constants.
//
// In the case where the number is small enough to not require a suffix, then
// exact integers will display without any decimal places and doubles will
// display with a least 1 digit after the decimal place (to indicate roundoff)
// and will use at least 2 sig figs. In that case, the longest possible string
// is size() == 7 (e.g., "-1111.1").
//
// If the number uses a suffix, then it will display exactly one decimal place
// (to indicate roundoff). In that case, the longest possible string is
// size() == 8 (e.g., "-1111.1M").
//
// If the number is more extreme than can be represented via an SI/IEC suffix,
// then it will display using scientific notation with two sig figs.  In that
// case, the longest possible string is size() == 9 (e.g., "-1.1e-300").
std::string HumanReadableNumber(double n, double one_k = 1024.0);

BENCHMARK_EXPORT
#if defined(__MINGW32__)
__attribute__((format(__MINGW_PRINTF_FORMAT, 1, 2)))
#elif defined(__GNUC__)
__attribute__((format(printf, 1, 2)))
#endif
std::string
StrFormat(const char* format, ...);

inline std::ostream& StrCatImp(std::ostream& out) BENCHMARK_NOEXCEPT {
  return out;
}

template <class First, class... Rest>
inline std::ostream& StrCatImp(std::ostream& out, First&& f, Rest&&... rest) {
  out << std::forward<First>(f);
  return StrCatImp(out, std::forward<Rest>(rest)...);
}

template <class... Args>
inline std::string StrCat(Args&&... args) {
  std::ostringstream ss;
  StrCatImp(ss, std::forward<Args>(args)...);
  return ss.str();
}

BENCHMARK_EXPORT
std::vector<std::string> StrSplit(const std::string& str, char delim);

// Disable lint checking for this block since it re-implements C functions.
// NOLINTBEGIN
#ifdef BENCHMARK_STL_ANDROID_GNUSTL
/*
 * GNU STL in Android NDK lacks support for some C++11 functions, including
 * stoul, stoi, stod. We reimplement them here using C functions strtoul,
 * strtol, strtod. Note that reimplemented functions are in benchmark::
 * namespace, not std:: namespace.
 */
unsigned long stoul(const std::string& str, size_t* pos = nullptr,
                    int base = 10);
int stoi(const std::string& str, size_t* pos = nullptr, int base = 10);
double stod(const std::string& str, size_t* pos = nullptr);
#else
using std::stod;   // NOLINT(misc-unused-using-decls)
using std::stoi;   // NOLINT(misc-unused-using-decls)
using std::stoul;  // NOLINT(misc-unused-using-decls)
#endif
// NOLINTEND

}  // end namespace benchmark

#endif  // BENCHMARK_STRING_UTIL_H_
