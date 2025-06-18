/**
 * @file comando.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

module;
#include <string>
#include <vector>
module ftp;

namespace ftp {

void Comando::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.controlSock.Send(GetReply(202));
}

} // namespace ftp