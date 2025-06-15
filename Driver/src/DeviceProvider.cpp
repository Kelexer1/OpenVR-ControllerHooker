#include "DeviceProvider.h"

#include "LogManager.h"
#include "SharedControllerMemory.h"
#include "globals.h"

vr::EVRInitError DeviceProvider::Init(vr::IVRDriverContext* pDriverContext) {
	vr::InitServerDriverContext(pDriverContext);

	// Initialize all essential components
	LogManager::initialize();
	HookManager::setupHooks((void*)vr::VRServerDriverHost());

	bool sharedMemoryResult = sharedMemoryIO.initialize(true);
	LogManager::log(LOG_INFO, "Shared memory initialization {0}", sharedMemoryResult ? "succeeded" : "failed");

	LogManager::log(LOG_INFO, "Initialized Device Provider");

	return vr::VRInitError_None;
}

void DeviceProvider::Cleanup() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

const char* const* DeviceProvider::GetInterfaceVersions() {
	return vr::k_InterfaceVersions;
}

void DeviceProvider::RunFrame() {}

bool DeviceProvider::ShouldBlockStandbyMode() { return false; }

void DeviceProvider::EnterStandby() {}

void DeviceProvider::LeaveStandby() {}
