/**
 * @file size.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/08/2024
 *
 *
 */

module;
#include <string>
#include <filesystem>
#include <sys/stat.h>
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void SIZE::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto archivo{args[0]};
#if __cpp_lib_format >= 201907L
	estado.controlSock.Send(std::vformat(
		GetReply(213),
		std::make_format_args(unmove(std::filesystem::file_size(archivo)))));
#else
	#if __cpp_lib_filesystem >= 201703L
	estado.controlSock.Send(format(GetReply(213), std::to_string(std::filesystem::file_size(archivo))));
	#else
	struct stat st{};
	if (stat(archivo.c_str(), &st) != 0) {
		throw std::runtime_error{std::strerror(errno)};
	}
	estado.controlSock.Send(format(GetReply(213), std::to_string(st.st_size)));
	#endif
#endif
}
} // namespace ftp