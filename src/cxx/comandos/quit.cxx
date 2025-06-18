/**
 * @file quit.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 19/08/2024
 *
 *
 */

module;
#include <string>
#include <vector>
module ftp;
import :common;

namespace ftp {

void QUIT::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.parar = true;
	estado.controlSock.Send(GetReplyCode(221));
}}