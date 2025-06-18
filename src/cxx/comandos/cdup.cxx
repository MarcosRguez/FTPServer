/**
 * @file cdup.cxx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#ifdef __has_include
	#if __has_include(<filesystem>)
		#include <filesystem>
	#endif
#endif
#include <unistd.h>
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void CDUP::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
#if __cpp_lib_filesystem >= 201703L
	std::filesystem::current_path(std::filesystem::current_path().parent_path());
#else
	if (chdir("..") != 0) {
		estado.controlSock.Send(GetReplyCode(550));
		std::perror("CDUP");
		// throw std::runtime_error{std::strerror(errno)};
	}
#endif
	estado.controlSock.Send(GetReplyCode(200));
}

} // namespace ftp