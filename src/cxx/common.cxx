/**
 * @file common.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

module;
#ifdef __has_include
	#if __has_include(<stdexcept>)
		#include <stdexcept>
	#endif
	#if __has_include(<format>)
		#include <format>
	#endif
	#if __has_include(<ranges>)
		#include <ranges>
	#endif
	#if __has_include(<sstream>)
		#include <sstream>
	#endif
	#if __has_include(<iostream>)
		#include <iostream>
	#endif
	#include <map>
	#include <memory>
	#include <filesystem>
	#include <netdb.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	// #include <mapa.hxx>
	#include "../../build/include/mapa.hxx"
#endif
module ftp;
import :mapas;
import :socket;
import :addrinfo;
import :comando;

namespace ftp {

using namespace std::literals;

auto ConectarSocket(const std::string& host, const int& port) -> Socket {
	AddrInfo info{
		host.c_str(),
		std::to_string(port).c_str(),
#if __cpp_designated_initializers >= 201707L
		addrinfo{
			.ai_flags{AI_NUMERICHOST | AI_NUMERICSERV},
			.ai_family{AF_INET},
			.ai_socktype{SOCK_STREAM},
			.ai_protocol{},
			.ai_addrlen{},
			.ai_addr{},
			.ai_canonname{},
			.ai_next{}}
#else
		addrinfo{
			AI_NUMERICHOST | AI_NUMERICSERV,
			AF_INET,
			SOCK_STREAM,
			0,
			0,
			0,
			0,
			0}
#endif
	};
	const auto& res{info.GetInfo()};
	Socket sock{res.ai_family, res.ai_socktype, res.ai_protocol};
	sock.Connect(*res.ai_addr);
	return sock;
}

#if __cpp_lib_filesystem >= 201703L
auto convert_file_time_to_mdtm_format(const std::filesystem::file_time_type& file_time) -> std::string {
	// Convertir file_time a un punto en el tiempo con sistema de tiempo Unix (desde epoch 1970)
	const auto sctp{std::chrono::time_point_cast<std::chrono::system_clock::duration>(file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now())};
	// Convertir a std::time_t (que es compatible con struct tm)
	const auto tt{std::chrono::system_clock::to_time_t(sctp)};
	// Convertir a struct tm para acceder a los componentes de la fecha y hora
	const auto* gmt{std::gmtime(&tt)};
	// Formatear la fecha y hora en el estándar MDTM (YYYYMMDDHHMMSS)
	std::ostringstream oss;
	oss << std::put_time(gmt, "%Y%m%d%H%M%S");
	return oss.str();
}
#endif

auto CrearMapaComandos() -> std::map<
#if __cpp_lib_string_view >= 201606L
	std::string_view,
#else
	std::string,
#endif
	std::unique_ptr<Comando>> {
	decltype(CrearMapaComandos())	resultado;
	mapa;
	return resultado;
}

#undef mapa

auto GetReplyCode(const int& num) -> std::string {
#if __cpp_lib_format >= 201907L
	return std::format("{} {}\r\n", num, FTPReplyCodes.at(num));
#else
	std::stringstream ss;
	ss << num << ' ' << FTPReplyCodes.at(num) << "\r\n";
	return ss.str();
#endif
}

// sock se puede acceder tras moverlo y es un peligro
void WaitForRequests(Socket&& sock) {
#if __cpp_designated_initializers >= 201707L
	FTPEstado estado{
		.controlSock{std::move(sock)},
		.dataSock{},
		.pasivo{false},
		.parar{false}};
#else
	FTPEstado estado{std::move(sock), Socket{}, false, false};
#endif
	const auto mapa{CrearMapaComandos()};
	estado.controlSock.Send(GetReplyCode(220));
	while (!estado.parar) {
		const auto línea{estado.controlSock.Recv<std::string>()};
		if (línea.empty()) { continue; }
#if __cpp_lib_ranges >= 201911L
		auto tokens{línea | std::views::split(' ')};
#else
		auto tokens{split(línea)};
#endif
#if __cpp_lib_string_view >= 201606L
		auto comando{
			std::string_view{
				(*tokens.begin()).data(),
				static_cast<std::string_view::size_type>(
	#if __cpp_lib_ranges >= 201911L
					std::ranges::distance(*tokens.begin())
	#else
					std::distance((*tokens.begin()).begin(), (*tokens.begin()).end())
	#endif
						)}};
#else
		auto comando{*tokens.begin()};
#endif
#if __cpp_lib_ranges_to_container >= 202202L
		auto args{tokens | std::views::drop(1) | std::ranges::to<std::vector<std::string>>()};
#else
	#if __cpp_lib_ranges >= 201911L
		auto range{tokens | std::views::drop(1)};
	#else
		auto range{tokens};
		range.erase(range.cbegin());
	#endif
		std::vector<std::string> args;
		for (const auto& i : range) {
			args.push_back(std::string{i.data()});
		}
#endif
		// Eliminar CRLF
		if (args.size() > 0) {
			args.back().resize(args.back().size() - 2);
		} else {
			comando = comando.substr(0, comando.size() - 2);
		}
		try {
			(*mapa.at(comando))(estado, args);
		} catch (const std::out_of_range& error) {
			estado.controlSock.Send(GetReplyCode(500));
		}
	}
	std::cout << "Cliente ha salido" << '\n';
}

} // namespace ftp