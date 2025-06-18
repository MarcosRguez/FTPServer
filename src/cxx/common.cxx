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
	#if __has_include(<print>)
		#include <print>
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
	decltype(CrearMapaComandos()) resultado;
	mapa;
	return resultado;
}

#undef mapa

constexpr auto GetReplyFMT(const int& code) ->
#if __cpp_lib_string_view >= 201606L
	std::string_view
#else
	std::string
#endif
{
	switch (code) {
		case 110: return "Restart marker reply.";
		case 120: return "Service ready in n minutes.";
		case 125: return "Data connection already open; transfer starting.";
		case 150: return "File status okay; about to open data connection.";
		case 200: return "Command okay.";
		case 202: return "Command not implemented, superfluous at this site.";
		case 211: return "System status, or system help reply.";
		case 212: return "Directory status.";
		case 213: return "{}";
		case 214: return "Help message.";
		case 215: return "{} system type.";
		case 220: return "Service ready for new user.";
		case 221: return "Service closing control connection.";
		case 225: return "Data connection open; no transfer in progress.";
		case 226: return "Closing data connection. Requested file action successful.";
		case 227: return "Entering Passive Mode ({})";
		case 229: return "Entering Extended Passive Mode (|||{}|)";
		case 230: return "User logged in, proceed.";
		case 231: return "User logged out; service terminated.";
		case 232: return "Logout command noted, but no transfer in progress.";
		case 250: return "Requested file action okay, completed.";
		case 257: return "\"{}\"";
		case 331: return "User name okay, need password.";
		case 332: return "Need account for login.";
		case 350: return "Requested file action pending further information.";
		case 421: return "Service not available, closing control connection.";
		case 425: return "Can't open data connection.";
		case 426: return "Connection closed; transfer aborted.";
		case 450: return "Requested file action not taken. File unavailable.";
		case 451: return "Requested action aborted: local error in processing.";
		case 452: return "Requested action not taken. Insufficient storage space in system.";
		case 500: return "Syntax error, command unrecognized.";
		case 501: return "Syntax error in parameters or arguments.";
		case 502: return "Command not implemented.";
		case 503: return "Bad sequence of commands.";
		case 504: return "Command not implemented for that parameter.";
		case 522: return "Network protocol not supported.";
		case 530: return "Not logged in.";
		case 532: return "Need account for storing files.";
		case 550: return "Requested action not taken. File unavailable.";
		case 551: return "Requested action aborted: page type unknown.";
		case 552: return "Requested file action aborted: exceeded storage allocation.";
		case 553: return "Requested action not taken. File name not allowed.";
		default: throw std::out_of_range{"Código de respuesta FTP no reconocido: " + std::to_string(code)};
	}
}

auto GetReply(const int& code) -> std::string {
#if __cpp_lib_format >= 201907L
	return std::format("{} {}\r\n", code, GetReplyFMT(code));
#else
	std::stringstream ss;
	ss << code << ' ' << GetReplyFMT(code) << "\r\n";
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
	estado.controlSock.Send(GetReply(220));
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
			estado.controlSock.Send(GetReply(500));
		}
	}
#if __cpp_lib_print >= 202207L
	std::println(std::cout, "Cliente ha salido");
#else
	std::cout << "Cliente ha salido" << '\n';
#endif
}

} // namespace ftp