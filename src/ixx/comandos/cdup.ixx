/**
 * @file cdup.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <vector>
#include <string>
export module ftp:comando.cdup;
import :comando;
import :estado;

export namespace ftp {

class CDUP : public Comando {
 public:
	void operator()(FTPEstado&, const std::vector<std::string>&) override;
};

} // namespace ftp