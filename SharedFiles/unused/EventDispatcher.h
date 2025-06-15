#pragma once
#include <functional>
#include <vector>
#include <openvr_driver.h>

#include "IControllerEventListener.h"

/**
 * @brief Represents various events that event listeners can recieve
 */
enum class EventType {
	PoseUpdated,
	ControllerConnected,
	ControllerDisconnected,
	OverrideEnabledChanged,
	TrackingResultChanged
};

/**
 * @brief The data that each event is accompanied by
 * 
 * Not all members are used for each event
 */
struct EventPayload {
	uint32_t deviceIndex;
	vr::DriverPose_t devicePose;
	vr::ETrackedControllerRole deviceRole;
	bool deviceShouldOverridePose;
	vr::ETrackingResult deviceTrackingResult;
};

/**
 * @brief Sends event pings to all associated event listeners
 */
class EventDispatcher {
public:
	/**
	 * @brief Registers an IControllerEventListener to recieve pings for certain events,
	 * such as when a pose is updated
	 * @param listener A pointer to the event listener that wants to recieve pings
	 */
	void addEventListener(IControllerEventListener* listener);

	/**
	 * @brief Dispatches event pings to all associated event listeners by calling the
	 * appropriate method
	 * 
	 * This method should not be called by the client and is instead called by the ControllerHooker
	 * driver
	 * 
	 * @param event The type of event that has occured
	 * @param payload A struct containing the data for the event
	 */
	void dispatch(EventType event, const EventPayload& payload);

private:
	/// The associated event listener for this EventDispatcher
	std::vector<IControllerEventListener*> listeners;
};