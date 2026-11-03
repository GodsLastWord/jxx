#include "jxxc/error_formatter.hpp"

namespace jxxc {

ErrorFormatter::ErrorFormatter(SourceText& source)
    : source{ source }
    , sourceName{ source.name() } {
}

const std::string ErrorFormatter::RED{ "\033[1;31m" };
const std::string ErrorFormatter::BOLD_WHITE{ "\033[1;38m" };
const std::string ErrorFormatter::CLEAR{ "\033[0m" };

} // namespace jxxc
