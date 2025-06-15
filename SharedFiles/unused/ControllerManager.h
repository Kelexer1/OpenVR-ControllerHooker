#pragma once
#include <vector>
#include <memory>

#include "Controller.h"

/**
 * @brief Manages multiple controller objects
 */
class ControllerManager {
public:
	/**
	 * @brief Constructs a new ControllerManager, and refreshes its stored controllers
	 */
	ControllerManager();

	~ControllerManager();

	///=========================
	/// Controller finders
	///=========================

	/**
	 * @brief Returns the first controller that matches the index, nullptr if none match
	 * @return A pointer to the controller, nullptr if none are found
	 */
	Controller* getControllerByIndex(const uint32_t deviceIndex);

	/**
	 * @brief Returns the first controller that matches the role, nullptr if none match
	 * @return A pointer to the controller, nullptr if none are found
	 */
	Controller* getControllerByRole(const vr::ETrackedControllerRole role);

	/**
	 * @brief Returns the first controller that matches the serial number, nullptr if none match
	 * @return A pointer to the controller, nullptr if none are found
	 */
	Controller* getControllerBySerial(const std::string serial);

	/**
	 * @brief Returns a vector containing unique pointers to all associated Controller objects
	 * @return A vector containing pointers to all associated controller objects
	 */
	const std::vector<std::unique_ptr<Controller>>& getAllControllers();

	/**
	 * @brief Scans all devices and constructs Controller objects for all valid controllers, then stores them
	 * @return The number of controllers that were detected
	 */
	const int refresh();

private:
	/// The associated controllers
	std::vector<std::unique_ptr<Controller>> controllers;
};