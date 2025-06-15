#pragma once
#include "IControllerEventListener.h"

/**
 * @brief Recieves event pings from the ControllerHooker driver
 */
class ControllerEventListener : public IControllerEventListener {
public:
	/**
	 * @brief Called when the ControllerHooker driver is requesting a pose
	 * 
	 * This method should be filled with your desired logic and always return a valid driver pose
	 * 
	 * @return The new pose
	 */
	ControllerPose poseRequested(uint32_t deviceIndex, ControllerPose naturalPose) override;

private:
	/// That last known update value for each device
	/// It is used to determine when a new value is available from shared memory
	/// Currently unused.
	//int32_t lastKnownCounter[MAX_DEVICES] = { 0 };
};