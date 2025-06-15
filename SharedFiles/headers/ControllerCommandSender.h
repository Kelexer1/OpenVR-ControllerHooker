#pragma once
#include "IControllerCommandSender.h"
#include "SharedControllerMemory.h"

/**
 * @brief Sends commands to the ControllerHooker driver by updating shared memory
 */
class ControllerCommandSender : public IControllerCommandSender {
public:
	/**
	 * @brief Constructs a new ControllerCommandSender associated with a ControllerManager
	 * @param controllerManager The associated controller manager
	 */
	ControllerCommandSender(SharedControllerMemoryIO* sharedMemory);

	///=========================
	/// Pose Manipulations
	///=========================

	/**
	 * @brief Fully overrides the stored pose for the controller with deviceIndex
	 *
	 * Note that this will override all stored positions, rotations, etc.
	 *
	 * @param deviceIndex The index of the controller
	 * @param newPose the pose that should override the current pose
	 */
	void setPose(const uint32_t deviceIndex, ControllerPose& pose) override;

	/**
	 * @brief Fills the vecPosition of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newPosition The new position
	 */
	void setPosition(const uint32_t deviceIndex, ControllerVector3& newPosition) override;

	/**
	 * @brief Fills the qRotation of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newRotation The new rotation
	 */
	void setRotation(const uint32_t deviceIndex, ControllerQuaternion& newRotation) override;

	/**
	 * @brief Fills the vecVelocity of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newVelocity The new velocity
	 */
	void setVelocity(const uint32_t deviceIndex, ControllerVector3& newVelocity) override;

	/**
	 * @brief Fills the vecAngularVelocity of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newAngularVelocity The new angular velocity
	 */
	void setAngularVelocity(const uint32_t deviceIndex, ControllerVector3& newAngularVelocity) override;

	/**
	 * @brief Fills the vecAcceleration of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newAcceleration The new acceleration
	 */
	void setAcceleration(const uint32_t deviceIndex, ControllerVector3& newAcceleration) override;

	/**
	 * @brief Fills the vecAngularAcceleration of the stored pose for the controller with deviceIndex
	 *
	 * Leaves the rest of the pose struct untouched
	 *
	 * @param deviceIndex The index of the controller
	 * @param newAngularAcceleration The new angular acceleration
	 */
	void setAngularAcceleration(const uint32_t deviceIndex, ControllerVector3& newAngularAcceleration) override;

	/**
	 * @brief Returns the stored pose of this controller
	 *
	 * Note that this pose is NOT nesessarily the actual pose of the controller,
	 * it is essentially a queued pose that can be applied to the controller
	 *
	 * @param deviceIndex The index of the controller
	 * @return The overriden pose of the controller, nullptr if something went wrong
	 */
	ControllerPose* getPose(const uint32_t deviceIndex) const override;

	/**
	 * @brief Returns the *actual* position of this controller
	 *
	 * Note that this pose may not equal the stored pose of the controller
	 *
	 * @param deviceIndex The index of the controller
	 * @return The natural pose of the controller, nullptr if something went wrong
	 */
	ControllerPose* getNaturalPose(const uint32_t deviceIndex) const override;

	///=========================
	/// Override control
	///=========================

	/**
	 * @brief Returns the stored pose of the controller with deviceIndex
	 *
	 * Note that this pose is NOT nesessarily the actual pose of the controller,
	 * it is essentially a queued pose that can be applied to the controller
	 *
	 * @param deviceIndex The index of the controller
	 * @param overridePose A boolean representing whether the controller should use it's overriden pose (true) or it's natural pose (false)
	 */
	void setUseCustomPose(const uint32_t deviceIndex, const bool shouldOverridePose) override;

	/**
	 * @brief Returns whether the controller is using a custom pose
	 *
	 * Note that this pose may not equal the stored pose of the controller
	 *
	 * @param deviceIndex The index of the controller
	 * @return True if the controller is using its overriden pose, false if it's using its natural pose, nullptr if something went wrong
	 */
	bool* getUseCustomPose(const uint32_t deviceIndex) const override;

	/**
	 * @brief Sets whether the controller should use ping-pong communication for pose manipulation
	 * @param deviceIndex The index of the controller
	 * @param usePingPong True if the controller should use ping-pong communication, false if it should not
	 */
	void setUsePingPong(const uint32_t deviceIndex, const bool usePingPong);

	/**
	 * @brief Returns whether the controller is currently using ping-pong communication for pose manipulation
	 * @param deviceIndex The index of the controller
	 * @return True if the controller is using ping-pong, false if it isn't, nullptr if something went wrong
	 */
	bool* getUsePingPong(const uint32_t deviceIndex);

private:
	/// The associated shared memory
	SharedControllerMemoryIO* sharedMemory;
};