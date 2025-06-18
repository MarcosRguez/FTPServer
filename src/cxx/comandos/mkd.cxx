/**
 * @file mkd.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/08/2024
 *
 *
 */

module;
#include <filesystem>
#if __has_include(<format>)
	#include <format>
#endif
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void MKD::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto dir{args[0]};
#if __cpp_lib_filesystem >= 201703L
	std::filesystem::create_directory(dir);
#else
	if (std::system(std::string{"mkdir "s + args[0]}.c_str()) != 0) {
		estado.controlSock.Send(GetReply(550));
		throw std::runtime_error{"no se pudo"};
	}
#endif
#if __cpp_lib_format >= 201907L
	estado.controlSock.Send(std::vformat(GetReply(257), std::make_format_args(dir)));
#else
	estado.controlSock.Send(format(GetReply(257), dir));
#endif
}
} // namespace ftp