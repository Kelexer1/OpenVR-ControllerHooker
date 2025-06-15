#pragma once
#include <openvr_driver.h>

/**
 * @brief Represents a single controller
 */
class Controller {
public:
	/**
	 * @brief Constructs a Controller object given a device index and role
	 * @param deviceIndex The unique index of the controller, assigned by OVR
	 * @param role The role of the controller, usually right or left
	 */
	Controller(uint32_t deviceIndex, vr::ETrackedControllerRole role);

	~Controller();

	///=========================
	/// Pose Manipulations
	///=========================

	/**
	 * @brief Fully overrides the stored pose
	 * 
	 * Note that this will override all stored positions, rotations, etc.
	 * 
	 * @param newPose the pose that should override the current pose
	 */
	void setPose(const vr::DriverPose_t& newPose);

	/**
	 * @brief Fills the vecPosition of the stored pose
	 * 
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newPosition The new position
	 */
	void setPosition(const vr::HmdVector3_t& newPosition);

	/**
	 * @brief Fills the qRotation of the stored pose
	 *
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newRotation The new rotation
	 */
	void setRotation(const vr::HmdQuaternion_t& newRotation);

	/**
	 * @brief Fills the vecVelocity of the stored pose
	 *
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newVelocity The new velocity
	 */
	void setVelocity(const vr::HmdVector3_t& newVelocity);

	/**
	 * @brief Fills the vecAngularVelocity of the stored pose
	 *
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newAngularVelocity The new angular velocity
	 */
	void setAngularVelocity(const vr::HmdVector3_t& newAngularVelocity);

	/**
	 * @brief Fills the vecAcceleration of the stored pose
	 *
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newAcceleration The new acceleration
	 */
	void setAcceleration(const vr::HmdVector3_t& newAcceleration);

	/**
	 * @brief Fills the vecAngularAcceleration of the stored pose
	 *
	 * Leaves the rest of the pose struct untouched
	 * 
	 * @param newAngularAcceleration The new angular acceleration
	 */
	void setAngularAcceleration(const vr::HmdVector3_t& newAngularAcceleration);

	/**
	 * @brief Returns the stored pose of this controller
	 *
	 * Note that this pose is NOT nesessarily the actual pose of the controller,
	 * it is essentially a queued pose that can be applied to the controller
	 * 
	 * @return The overriden pose
	 */
	const vr::DriverPose_t& getPose() const;

	/**
	 * @brief Returns the *actual* position of this controller
	 *
	 * Note that this pose may not equal the stored pose of the controller
	 * 
	 * @return The natural pose
	 */
	const vr::DriverPose_t& getNaturalPose() const;

	///=========================
	/// Override control
	///=========================

	/**
	 * @brief Sets whether this controller should use the stored pose or unaltered pose
	 * @param overridePose A boolean representing whether the controller should use it's overriden pose (true) or it's natural pose (false)
	 */
	void setUseCustomPose(const bool overridePose);

	/**
	 * @brief Returns whether this controller is using the stored pose or unaltered pose
	 * @return True if this controller is using it's overriden pose, false if it's using its natural pose
	 */
	bool getUseCustomPose() const;

	///=========================
	/// Accessors
	///=========================

	/**
	 * @brief Returns the device index of this controller
	 * @return The device index
	 */
	const uint32_t getIndex() const;

	/**
	 * @brief Returns the role of this controller
	 * 
	 * This role is either left, right, or invalid in the event of an error
	 * 
	 * @return The role of the controller
	 */
	const vr::ETrackedControllerRole getRole() const;

	/**
	 * @brief Returns the serial number of this controller
	 */
	const std::string getSerial() const;

private:
	/// The device index for this controller
	uint32_t index;

	/// The role associated with this controller (left, right, invalid)
	vr::ETrackedControllerRole role;

	/// The serial number associated with this controller
	std::string serial;

	/// Dictates whether this controller should apply its unmodified pose (natural hand 
	/// position), or its overriden pose
	bool useCustomPose;

	/// The overriden pose of this controller. This pose is *not* nesessarily equal
	/// to the actual pose of this controller. This pose is applied iff <overridePose>
	/// is true
	vr::DriverPose_t overridenPose;

	/// The actual/natural pose of this controller. This pose is the pose that would normally
	/// be applied to this controller (the pose of the physical controller)
	vr::DriverPose_t naturalPose;
};