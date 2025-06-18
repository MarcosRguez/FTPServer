/**
 * @file eprt.cc
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 22/08/2024
 *
 *
 */

module;
#include <string>
#include <ranges>
#include <filesystem>
#include <string>
module ftp;
import :mapas;
import :common;
import :utilidades;

using namespace std::literals;

namespace ftp {

void EPRT::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto arg{args[0]};
#if __cpp_lib_ranges >= 201911L
	auto sep{arg | std::views::split('|')};
#else
	auto sep{split(arg, '|')};
#endif
	auto it{std::next(sep.begin())};
#if __cpp_lib_ranges >= 201911L
	const auto net_ptr{to_string_from_range(*it++)};
	const auto net_addr{to_string_from_range(*it++)};
	const auto tcp_port{to_string_from_range(*it++)};
#else
	const auto net_ptr{*it++};
	const auto net_addr{*it++};
	const auto tcp_port{*it++};
#endif
	if (std::stoi(net_ptr.data()) != 1) {
		estado.controlSock.Send(GetReplyCode(522));
		return;
	}
	estado.dataSock = ConectarSocket(net_addr, std::stoi(tcp_port.data()));
	estado.pasivo = false;
	estado.controlSock.Send(GetReplyCode(200));
}

}