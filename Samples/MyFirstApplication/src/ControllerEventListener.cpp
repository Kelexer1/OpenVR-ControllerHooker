#include "ControllerEventListener.h"

// We don't actually need to implement this method for a basic application, so we will leave it
// It is used when the ControllerHooker driver requests a pose to apply (ping-pong communication)
ControllerPose ControllerEventListener::poseRequested(uint32_t deviceIndex, ControllerPose naturalPose) {
	return naturalPose;
}