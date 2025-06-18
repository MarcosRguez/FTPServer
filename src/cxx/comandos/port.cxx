/**
 * @file port.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 20/08/2024
 *
 *
 */

module;
#include <filesystem>
#include <ranges>
module ftp;
import :common;

using namespace std::literals;

namespace ftp {

void PORT::operator()(
	FTPEstado& estado,
	[[maybe_unused]] const std::vector<std::string>& args) {
#if __cpp_lib_ranges >= 201911L
	auto hostport{std::views::split(args[0], ',')};
#else
	auto hostport{split(args[0], ',')};
#endif
#if __cpp_lib_ranges_to_container >= 202202L
	const auto host{
		hostport |
		std::views::take(4) |
		std::views::join_with('.') |
		std::ranges::to<std::string>()};
	auto port{hostport | std::views::drop(4) | std::views::take(2)};
	const int puerto{
		std::stoi(std::ranges::to<std::string>(*port.begin())) * 256 +
		std::stoi(std::ranges::to<std::string>(*std::next(port.begin())))};
#else
	std::string host;
	auto it{hostport.begin()};
	for (auto i{0}; i < 4 && it != hostport.end(); ++i, ++it) {
		for (char c : *it) {
			host += c;
		}
		if (i != 3) {
			host += '.';
		}
	}
	auto it4{std::next(hostport.begin(), 4)};
	auto it5{std::next(hostport.begin(), 5)};
	int port1{0};
	int port2{0};
	for (char c : *it4) {
		port1 = port1 * 10 + (c - '0');
	}
	for (char c : *it5) {
		port2 = port2 * 10 + (c - '0');
	}
	const int puerto{port1 * 256 + port2};
#endif
	estado.dataSock = ConectarSocket(host, puerto);
	estado.pasivo = false;
	estado.controlSock.Send(GetReply(200));
}
} // namespace ftp