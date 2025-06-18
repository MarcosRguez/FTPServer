/**
 * @file user.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 13/08/2024
 *
 *
 */

module;
#include <string>
#include <vector>
module ftp;

namespace ftp {

void USER::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto nombre{args[0]};
	estado.controlSock.Send(GetReply(331));
}
} // namespace ftp