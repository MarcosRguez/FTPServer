/**
 * @file pwd.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 13/08/2024
 *
 *
 */

module;
#include <filesystem>
#if __has_include(<format>)
	#include <format>
#endif
#include <unistd.h>
module ftp;

using namespace std::literals;

namespace ftp {

void PWD::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
#if __cpp_lib_format >= 201907L
	estado.controlSock.Send(
		std::vformat(
			GetReply(257),
			std::make_format_args(unmove(std::filesystem::current_path().string()))));
#else
	#if __cpp_lib_filesystem >= 201703L
	estado.controlSock.Send(format(GetReply(257), std::filesystem::current_path().string()));
	#else
	std::unique_ptr<char[], decltype(&std::free)> cwd{get_current_dir_name(), std::free};
	estado.controlSock.Send(format(GetReply(257), std::string{cwd.get()}));
	#endif
#endif
}
} // namespace ftp