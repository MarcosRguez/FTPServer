/**
 * @file cwd.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 19/08/2024
 *
 *
 */

module;
#include <filesystem>
#include <string>
#include <unistd.h>
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void CWD::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
#if __cpp_lib_filesystem >= 201703L
	std::filesystem::current_path(args[0]);
#else
	if (chdir(args.front().c_str()) != 0) {
		estado.controlSock.Send(GetReplyCode(550));
		std::perror("CWD");
		// throw std::runtime_error{std::strerror(errno)};
	}
#endif
	estado.controlSock.Send(GetReplyCode(250));
}
}