/**
 * @file pass.cpp
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
import :mapas;

namespace ftp {

void PASS::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto contraseña{args.size() == 1 ? args[0] : ""};
	estado.controlSock.Send(GetReplyCode(230));
}}