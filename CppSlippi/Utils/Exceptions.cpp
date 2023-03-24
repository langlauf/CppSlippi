#include "Exceptions.h"

#include <filesystem>
#include <format>


namespace cpp_slippi::utils {
namespace {

std::string formatMessage(std::string_view msg, std::source_location loc) {
	std::filesystem::path path(loc.file_name());
	return std::format("[{}:{}] {}", path.filename().string(), loc.line(), msg);
}

}  // namespace

BaseException::BaseException(std::string_view msg, std::source_location loc)
	: what_(formatMessage(msg, loc)) {}

}  // namespace cpp_slippi::utils
