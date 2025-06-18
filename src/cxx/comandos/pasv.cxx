/**
 * @file pasv.cpp
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
#include <algorithm>
#include <cstdio>
#include <netinet/in.h>
module ftp;
import :mapas;
import :estado;
import :common;
import :utilidades;

using namespace std::literals;

namespace ftp {

void PASV::operator()(
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
	const sockaddr_in sin{AF_INET, htons(0), in_addr{INADDR_ANY}, 0};
#endif
	estado.dataSock.Bind(*reinterpret_cast<const sockaddr*>(&sin));
	estado.dataSock.Listen(5);
	estado.pasivo = true;
	auto resultado{estado.controlSock.GetAddr<std::string>()};
#if __cpp_lib_ranges >= 201911L
	std::ranges::replace(resultado, '.', ',');
#else
	replace(resultado, '.', ',');
#endif
	const auto puerto{ntohs(estado.dataSock.GetPort())};
#if __cpp_lib_format >= 201907L
	resultado.append(
		std::vformat(
			",{},{}",
			std::make_format_args(
				unmove(puerto / 256),
				unmove(puerto % 256))));
	estado.controlSock.Send(std::vformat(GetReplyCode(227), std::make_format_args(resultado)));
#else
	resultado.push_back(',');
	resultado.append(std::to_string(puerto / 256));
	resultado.push_back(',');
	resultado.append(std::to_string(puerto % 256));
	estado.controlSock.Send(format(GetReplyCode(227), resultado));
#endif
} // en c++14 y c++17 se produce en segfault al salir de esta función
}