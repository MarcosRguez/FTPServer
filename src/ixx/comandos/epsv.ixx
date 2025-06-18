/**
 * @file epsv.hpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 24/08/2024
 *
 *
 */

module;
#include <vector>
#include <string>
export module ftp:comando.epsv;
import :comando;
import :estado;

export namespace ftp {

class EPSV : public Comando {
 public:
	void operator()(FTPEstado&, const std::vector<std::string>&) override;
};

} // namespace ftp