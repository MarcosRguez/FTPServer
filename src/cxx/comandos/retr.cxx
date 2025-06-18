/**
 * @file retr.cpp
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
#include <vector>
module ftp;
import :common;
import :utilidades;
import :estado;

using namespace std::literals;

namespace ftp {

void RETR::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	const auto archivo{CargarFichero<std::string>(args[0])};
	if (estado.pasivo) {
		estado.controlSock.Send(GetReply(150));
		auto coso{estado.dataSock.Accept()};
#if __cpp_lib_span >= 202002L
		coso.Send(std::span{archivo});
#else
		coso.Send(archivo);
#endif
		estado.controlSock.Send(GetReply(226));
	} else {
		estado.controlSock.Send(GetReply(125));
#if __cpp_lib_span >= 202002L
		estado.dataSock.Send(std::span{archivo});
#else
		estado.dataSock.Send(archivo);
#endif
		estado.controlSock.Send(GetReply(226));
		estado.dataSock.~Socket();
	}
}
} // namespace ftp