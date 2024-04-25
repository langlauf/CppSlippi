#pragma once

#include <exception>
#include <source_location>
#include <string>
#include <string_view>

#include "CppSlippi/Utils/Defines.h"


namespace cpp_slippi::utils {

CPP_SLIPPI_CLASS_API
class CPP_SLIPPI_API BaseException : public std::exception {
public:
	explicit BaseException(std::string_view msg, std::source_location loc);

	const char* what() const noexcept final {
		return what_.c_str();
	}

private:
	std::string what_;
};


// Defines a new exception type that inherits from BaseException with no new
// behavior.
#define CPP_SLIPPI_DEFINE_EXCEPTION(ExceptionName)                                  \
    class CPP_SLIPPI_API ExceptionName : public cpp_slippi::utils::BaseException {  \
    public:                                                                         \
        explicit ExceptionName(                                                     \
            std::string_view msg,                                                   \
            std::source_location loc = std::source_location::current()) :           \
                cpp_slippi::utils::BaseException(msg, loc) {}                       \
    }

}  // namespace cpp_slippi::utils
