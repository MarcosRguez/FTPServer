/**
 * @file syst.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 15/08/2024
 *
 *
 */

module;
#include <vector>
#include <string>
module ftp;

using namespace std::literals;

namespace ftp {

void SYST::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.controlSock.Send(format(GetReply(215), "LINUX"s));
}
} // namespace ftp