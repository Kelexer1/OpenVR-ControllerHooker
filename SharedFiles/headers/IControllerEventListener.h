#pragma once
#include "ControllerTypes.h"

#include <cstdint>

/**
 * @brief An abstract interface for recieving event pings from the ControllerHooker driver
 * 
 * This class should *not* be instanciated directly. Instead, inherit from it and implement its methods
 */
class IControllerEventListener {
public:
	virtual ~IControllerEventListener() = default;

	/**
	 * @brief Called when the ControllerHooker driver is requesting a pose
	 *
	 * This method should be filled with your desired logic and always return a valid driver pose
	 *
	 * @return The new pose
	 */
	virtual ControllerPose poseRequested(uint32_t deviceIndex, ControllerPose naturalPose) = 0;

	///=========================
	/// Unused Methods (May be implemented later)
	///=========================

	/**
	 * @brief Called when the pose for a controller is updated
	 * @param deviceIndex The index of the controller that has its position updated
	 * @param pose The new pose of the controller
	 */
	//virtual void onPoseUpdated(uint32_t deviceIndex, vr::DriverPose_t pose) = 0;

	/**
	 * @brief Called when a controller connects (such as when it's turned on)
	 * @param deviceIndex The index of the controller that connected
	 * @param role The role of the controller that connected
	 */
	//virtual void onControllerConnected(uint32_t deviceIndex, vr::ETrackedControllerRole role) = 0;

	/**
	 * @brief Called when a controller disconnects (such as when it's turned off)
	 * @param deviceIndex The index of the controller that disconnected
	 */
	//virtual void onControllerDisconnected(uint32_t deviceIndex) = 0;

	/**
	 * @brief Called when the controller starts using its overriden pose or natural pose
	 * @param deviceIndex The index of the controller
	 * @param useCustomPose True if the controller is using its overriden pose, false if it's using its natural pose
	 */
	//virtual void onOverrideEnabledChanged(uint32_t deviceIndex, bool useCustomPose) = 0;

	/**
	 * @brief Called when the tracking result of a controller changes (success -> error etc.)
	 * @param deviceIndex The index of the controller
	 * @param result The new tracking result of the controller
	 */
	//virtual void onTrackingResultChanged(uint32_t deviceIndex, vr::ETrackingResult result) = 0;
};