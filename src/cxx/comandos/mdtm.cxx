/**
 * @file mdtm.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 29/08/2024
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
import :utilidades;

using namespace std::literals;

namespace ftp {

void MDTM::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
#if __cpp_lib_format >= 201907L
	estado.controlSock.Send(
		std::vformat(
			GetReply(213),
			std::make_format_args(
				unmove(
	#if __cpp_lib_filesystem >= 201703L
					convert_file_time_to_mdtm_format(std::filesystem::last_write_time(args[0]))
	#else
					"🐱"s
	#endif
						))));
#else
	#if __cpp_lib_filesystem >= 201703L
	estado.controlSock.Send(unmove(convert_file_time_to_mdtm_format(std::filesystem::last_write_time(args[0]))));
	#else
	estado.controlSock.Send("🐱");
	#endif
#endif
}
} // namespace ftp