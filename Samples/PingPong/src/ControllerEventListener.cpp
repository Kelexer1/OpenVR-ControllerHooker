#include "ControllerEventListener.h"

// This method will be called every time a controller wants to update its position
ControllerPose ControllerEventListener::poseRequested(uint32_t deviceIndex, ControllerPose naturalPose) {
	// naturalPose is the pose that the controller wants to set to (aka probably your hand)
	// Lets add a fixed offset to the controller to make it 1m higher than it normally would be

	// vecPosition[1] corresponds to the Y axis (vertical)
	naturalPose.vecPosition[1] += 1.0;

	// return the modified naturalPose
	return naturalPose;

	// Quick note: We dont need to return naturalPose, or even touch it at all
	// we just need to return a valid ControllerPose struct
}