/**
 * @file pwd.hpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 13/08/2024
 *
 *
 */

module;
#include <vector>
#include <string>
export module ftp:comando.pwd;
import :comando;
import :estado;

export namespace ftp {

class PWD : public Comando {
 public:
	void operator()(FTPEstado&, const std::vector<std::string>&) override;
};}