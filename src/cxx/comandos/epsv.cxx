/**
 * @file epsv.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 24/08/2024
 *
 *
 */

module;
#include <string>
#include <ranges>
#include <filesystem>
#include <string>
#include <iostream>
#include <netinet/in.h>
module ftp;
import :mapas;
import :estado;
import :common;
import :utilidades;

using namespace std::literals;

namespace ftp {

void EPSV::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.dataSock = Socket{AF_INET, SOCK_STREAM, 0};
#if __cpp_designated_initializers >= 201707L
	const sockaddr_in sin{
		.sin_family{AF_INET},
		.sin_port{htons(0)},
		.sin_addr{.s_addr{INADDR_ANY}},
		.sin_zero{}};
#else
	const sockaddr_in sin{AF_INET, htons(0), estado.controlSock.GetAddr<in_addr>(), 0};
#endif
	estado.dataSock.Bind(*reinterpret_cast<const sockaddr*>(&sin));
	estado.dataSock.Listen(5);
	estado.pasivo = true;
#if __cpp_lib_format >= 201907L
	estado.controlSock.Send(std::vformat(GetReplyCode(229), std::make_format_args(unmove(estado.dataSock.GetPort()))));
#else
	estado.controlSock.Send(format(GetReplyCode(229), std::to_string(ntohs(estado.dataSock.GetPort()))));
#endif
}
} // namespace ftp