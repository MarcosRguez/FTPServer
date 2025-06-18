/**
 * @file stor.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 26/08/2024
 *
 *
 */

module;
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sys/socket.h>
module ftp;
import :estado;
import :common;

namespace ftp {

void STOR::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	std::string archivo;
	if (estado.pasivo) {
		estado.controlSock.Send(GetReply(150));
		auto coso{estado.dataSock.Accept()};
		archivo = coso.Recv<std::string>(MSG_DONTWAIT);
		estado.controlSock.Send(GetReply(226));
	} else {
		estado.controlSock.Send(GetReply(125));
		archivo = estado.dataSock.Recv<std::string>(MSG_DONTWAIT);
		estado.controlSock.Send(GetReply(226));
		estado.dataSock.~Socket();
	}
	std::fstream fs{args[0], std::fstream::out};
	if (!fs.is_open()) { throw std::runtime_error{"💽"}; }
	fs << archivo;
}
} // namespace ftp