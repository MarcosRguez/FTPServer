/**
 * @file type.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 20/08/2024
 *
 *
 */

module;
#include <vector>
#include <string>
module ftp;
import :mapas;
import :common;

namespace ftp {

void TYPE::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.controlSock.Send(GetReplyCode(200));
}}