#include "ControllerManager.h"

ControllerManager::ControllerManager() {
	// Scan for new controllers
	this->refresh();
}

ControllerManager::~ControllerManager() {}

Controller* ControllerManager::getControllerByIndex(const uint32_t deviceIndex) {
	for (const std::unique_ptr<Controller>& controller : controllers) {
		if (controller->getIndex() == deviceIndex) {
			return controller.get();
		}
	}
	return nullptr;
}

Controller* ControllerManager::getControllerByRole(const vr::ETrackedControllerRole role) {
	for (const std::unique_ptr<Controller>& controller : controllers) {
		if (controller->getRole() == role) {
			return controller.get();
		}
	}
	return nullptr;
}

Controller* ControllerManager::getControllerBySerial(const std::string serial) {
	for (const std::unique_ptr<Controller>& controller : controllers) {
		if (controller->getSerial() == serial) {
			return controller.get();
		}
	}
	return nullptr;
}

const std::vector<std::unique_ptr<Controller>>& ControllerManager::getAllControllers() {
	return controllers;
}

const int ControllerManager::refresh() {
	controllers.clear();
	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
		vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer(i);
		vr::ETrackedPropertyError error;
		int32_t deviceClass = vr::VRProperties()->GetInt32Property(container, vr::Prop_DeviceClass_Int32, &error);
		if (error == vr::TrackedProp_Success && 
			static_cast<vr::ETrackedDeviceClass>(deviceClass) == vr::TrackedDeviceClass_Controller) {
			int32_t role = vr::VRProperties()->GetInt32Property(container, vr::Prop_ControllerRoleHint_Int32, &error);
			if (error == vr::TrackedProp_Success) {
				controllers.push_back(std::make_unique<Controller>(i, static_cast<vr::ETrackedControllerRole>(role)));
			}
		}
	}
	return controllers.size();
}