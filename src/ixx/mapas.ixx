/**
 * @file mapas.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief 
 * @version 0.1
 * @date 17/06/2025
 * 
 * 
 */

module;
#ifdef __has_include
	#include <map>
	#if __has_include(<string_view>)
		#include <string_view>
	#else
		#error "error"
	#endif
#endif
export module ftp:mapas;

export namespace ftp {

const std::map<
	int,
#if __cpp_lib_string_view >= 201606L
	std::string_view
#else
	std::string
#endif
	>
	FTPReplyCodes{
		{110, "Restart marker reply."},
		{120, "Service ready in n minutes."},
		{125, "Data connection already open; transfer starting."},
		{150, "File status okay; about to open data connection."},
		{200, "Command okay."},
		{202, "Command not implemented, superfluous at this site."},
		{211, "System status, or system help reply."},
		{212, "Directory status."},
		{213, "{}"},
		{214, "Help message."},
		{215, "{} system type."},
		{220, "Service ready for new user."},
		{221, "Service closing control connection."},
		{225, "Data connection open; no transfer in progress."},
		{226, "Closing data connection. Requested file action successful."},
		{227, "Entering Passive Mode ({})"},
		{229, "Entering Extended Passive Mode (|||{}|)"},
		{230, "User logged in, proceed."},
		{231, "User logged out; service terminated."},
		{232, "Logout command noted, but no transfer in progress."},
		{250, "Requested file action okay, completed."},
		{257, "\"{}\""},
		{331, "User name okay, need password."},
		{332, "Need account for login."},
		{350, "Requested file action pending further information."},
		{421, "Service not available, closing control connection."},
		{425, "Can't open data connection."},
		{426, "Connection closed; transfer aborted."},
		{450, "Requested file action not taken. File unavailable."},
		{451, "Requested action aborted: local error in processing."},
		{452, "Requested action not taken. Insufficient storage space in system."},
		{500, "Syntax error, command unrecognized."},
		{501, "Syntax error in parameters or arguments."},
		{502, "Command not implemented."},
		{503, "Bad sequence of commands."},
		{504, "Command not implemented for that parameter."},
		{522, "Network protocol not supported."},
		{530, "Not logged in."},
		{532, "Need account for storing files."},
		{550, "Requested action not taken. File unavailable."},
		{551, "Requested action aborted: page type unknown."},
		{552, "Requested file action aborted: exceeded storage allocation."},
		{553, "Requested action not taken. File name not allowed."}};

}