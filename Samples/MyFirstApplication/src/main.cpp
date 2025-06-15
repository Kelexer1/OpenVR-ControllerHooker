// ControllerHooker header files
#include "ControllerCommandSender.h"
#include "ControllerEventListener.h"
#include "SharedControllerMemory.h"

#include <openvr.h>
#include <iostream>
#include <vector>
#include <thread>

int main() {
	///=========================
	/// Initializing OpenVR
	///=========================

	// Initialize OpenVR (out of scope for this sample)
	vr::EVRInitError error = vr::VRInitError_None;

	auto VR_Init = vr::VR_Init(&error, vr::EVRApplicationType::VRApplication_Background);
	if (error != vr::VRInitError_None) {
		std::cerr << "Failed to initialize OpenVR: " << vr::VR_GetVRInitErrorAsEnglishDescription(error) << std::endl;
		return -1;
	}

	///=========================
	/// Initializing ControllerHooker
	///=========================

	// Now, we can initialize ControllerHooker
	// We need to initialize the event listener, the shared memory IO, and the command sender

	// The event listener is responsible for recieving event notifications from the driver, and responding to ping-pong communication requests
	ControllerEventListener myEventListener;

	// The shared memory IO is responsible for reading and writing shared memory, you shoulden't need to directly interface with it
	SharedControllerMemoryIO mySharedMemory(&myEventListener);

	// We need to initialize the shared memory, which will return true if the initialization was a success
	if (!mySharedMemory.initialize()) {
		std::cerr << "Shared memory failed to initialize" << std::endl;
		return -1;
	}

	// The command sender is a sort of wrapper for the shared memory IO, and provides a more user friendly interface for pose manipulation
	ControllerCommandSender myCommandSender(&mySharedMemory);

	///=========================
	/// Simple controller collector
	///=========================

	// At this point, everything that needs to be set up or initialized has been completed

	// Last, lets find all device indexes that correspond to controllers
	// The ControllerHooker client API uses device index to differentiate between controllers
	std::vector<uint32_t> controllerIndexes;
	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
		auto role = VR_Init->GetControllerRoleForTrackedDeviceIndex(i);

		// Lets store the controllers device index for later use
		if (role == vr::TrackedControllerRole_LeftHand || role == vr::TrackedControllerRole_RightHand) {
			controllerIndexes.push_back(i);
			std::clog << "Controller " << i << " is connected" << std::endl;
		}
	}

	// Lets exit the program if no controllers were found. In a full application, you could have a rescanning loop
	if (controllerIndexes.size() == 0) {
		std::cerr << "No controllers found" << std::endl;
	}

	///=========================
	/// Using ControllerHooker
	///=========================

	// Now that all the initialization and set up is complete, we can actually interact with the controllers
	ControllerVector3 newPosition = { 0.f, 0.f, 0.f }; // Define a constant overriden position (world origin)
	for (uint32_t deviceIndex : controllerIndexes) {
		auto initial_pose = myCommandSender.getNaturalPose(deviceIndex);

		// Ensure that we actually read the natural pose properly
		if (initial_pose != nullptr) {
			myCommandSender.setPose(deviceIndex, *initial_pose);	// Initialize the overriden pose to be its natural pose (by default the overriden pose is blank)
			myCommandSender.setPosition(deviceIndex, newPosition);	// Override the position field of its pose
			myCommandSender.setUseCustomPose(deviceIndex, true);	// Make the controller use its overriden pose
		}
	}

	return 0;
}