#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <fmt/core.h>

/**
 * @brief Types of logs
 * 
 * Logs contain a prefix such as "[INFO]" that correspond to each LogType
 */
enum LogType {
	LOG_INFO,
	LOG_DEBUG,
	LOG_ERROR
};

/**
 * @brief Manages file logging throughout the driver
 */
class LogManager {
public:
	/**
	 * @brief Initializes the output file (C:\temp\log_ControllerEmulator.txt)
	 * @return true if initialization was successful, false otherwise
	 */
	static bool initialize();

	/**
 	 * @brief Logs a formatted message with a given logtype
	 * 
	 * Example usage:
	 * @code
	 * LogManager::log(LOG_INFO, "Sample log with code: {}", 1)
	 * @endcode
	 */
	template <typename... Args>
	static void log(LogType logType, const std::string& formatString, Args&&... args) {
		// If the log stream hasn't been initialized we silently fail
		if (!initialized) {
			return;
		}

		// Convert the logType enum to a string prefix
		std::string prefix;
		switch (logType) {
		case LOG_INFO:
			prefix = "[INFO]  ";
			break;
		case LOG_DEBUG:
			prefix = "[DEBUG] ";
			break;
		case LOG_ERROR:
			prefix = "[ERROR] ";
			break;
		default:
			prefix = "[OTHER] ";
		}

		// Output the formatted message
		std::string message = fmt::format(formatString, std::forward<Args>(args)...);
		std::cout << prefix << message << std::endl;
	}

	/**
	 * @brief Logs and buffered messages and closes the output file
	 */
	static void shutdown();
private:
	/// Log file stream
	static std::ofstream logFile;

	/// Whether the log file stream has been initialized
	static bool initialized;
};