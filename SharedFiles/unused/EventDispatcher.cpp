#include "EventDispatcher.h"

void EventDispatcher::addEventListener(IControllerEventListener* listener) {
	if (listener && std::find(listeners.begin(), listeners.end(), listener) == listeners.end()) {
		listeners.push_back(listener);
	}
}

void EventDispatcher::dispatch(EventType event, const EventPayload& payload) {
	//for (auto* listener : listeners) {
	//	switch (event) {
	//	case EventType::PoseUpdated:
	//		listener->onPoseUpdated(payload.deviceIndex, payload.devicePose);
	//		break;
	//	case EventType::ControllerConnected:
	//		listener->onControllerConnected(payload.deviceIndex, payload.deviceRole);
	//		break;
	//	case EventType::ControllerDisconnected:
	//		listener->onControllerDisconnected(payload.deviceIndex);
	//		break;
	//	case EventType::OverrideEnabledChanged:
	//		listener->onOverrideEnabledChanged(payload.deviceIndex, payload.deviceShouldOverridePose);
	//		break;
	//	case::EventType::TrackingResultChanged:
	//		listener->onTrackingResultChanged(payload.deviceIndex, payload.deviceTrackingResult);
	//	}
	//}
}