/**
 * @file list.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 26/08/2024
 *
 *
 */

module;
#include <filesystem>
#include <set>
#include <numeric>
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void LIST::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
	estado.controlSock.Send(GetReply(125));
	std::vector<std::string> lista;
#if __cpp_lib_filesystem >= 201703L
	for (const auto& i : std::filesystem::directory_iterator{std::filesystem::current_path()}) {
		lista.emplace_back(i.path().filename().string() + "\r\n");
	}
	#if __cpp_lib_ranges >= 201911L
	std::ranges::sort(lista);
	#else
	std::sort(lista.begin(), lista.end());
	#endif
#else
	std::unique_ptr<FILE, decltype(&pclose)> pipe{popen("ls -l", "r"), pclose};
	if (!pipe) {
		throw std::runtime_error("Error al ejecutar el comando.");
	}
	constexpr auto búferSize{64};
	char búfer[búferSize]; // ha yque darle tamaño al búfer,
	std::string str;			 // hay que parar en cada salto de línea y hacer push
	while (std::fgets(búfer, búferSize, pipe.get()) != nullptr) {
		str += búfer;
	}
	lista = split(str, '\n');
	lista.pop_back();
	for (auto& i : lista) {
		i.append("\r\n");
	}
#endif
	const auto resultado{std::accumulate(lista.begin(), lista.end(), std::string{})};
	if (estado.pasivo) {
		auto coso{estado.dataSock.Accept()};
		coso.Send(resultado);
		estado.controlSock.Send(GetReply(226));
	} else {
		estado.dataSock.Send(resultado);
		estado.controlSock.Send(GetReply(226));
		estado.dataSock.~Socket();
	}
}
} // namespace ftp