#include "HookManager.h"

#include "vrmath.h"

#include "LogManager.h"
#include "globals.h"
#include "SharedControllerMemory.h"
#include "Utils.h"

// Default initializations, will be overriden later
HINSTANCE HookManager::hModule = nullptr;

// Hook Declarations
typedef void(*_TrackedDevicePoseUpdated)(void* _this, uint32_t unWhichDevice, const vr::DriverPose_t& newPose, uint32_t unPoseStructSize);
static _TrackedDevicePoseUpdated originalTrackedDevicePoseUpdated = nullptr;

void overrideTrackedDevicePoseUpdated(void* _this, uint32_t unWhichDevice, const vr::DriverPose_t& newPose, uint32_t unPoseStructSize) {
	vr::DriverPose_t overridePose = newPose;

	// Only work with controllers
	if (getDeviceClass(unWhichDevice) == vr::TrackedDeviceClass_Controller) {
		// Update natural pose
		bool active;
		if (sharedMemoryIO.isControllerSlotActive(unWhichDevice, active) && active) {
			sharedMemoryIO.writeNaturalPose(unWhichDevice, FromDriverPose(newPose));
		}

		// Check if using ping-pong communication
		bool usePingPong;
		if (sharedMemoryIO.readUsePingPong(unWhichDevice, usePingPong) && usePingPong) {
			overridePose = ToDriverPose(sharedMemoryIO.pingPong(unWhichDevice));
		} else {
			// Check if using a custom pose
			bool useOverridePose;
			if (sharedMemoryIO.readUseCustomPose(unWhichDevice, useOverridePose) && useOverridePose) {
				// Static read
				ControllerPose customPose;
				if (sharedMemoryIO.readOverridenPose(unWhichDevice, customPose)) {
					overridePose = ToDriverPose(customPose);
				}
			}
		}
	}

	// Call the original TrackedDevicePoseUpdated()
	if (originalTrackedDevicePoseUpdated) {
		(originalTrackedDevicePoseUpdated)(_this, unWhichDevice, overridePose, unPoseStructSize);
	}
}

void HookManager::setupHooks(void* host) {
	if (!host) {
		LogManager::log(LOG_ERROR, "Host is not initialized");
		return;
	}

	void** vtable = *(void***)host;
	if (!vtable) {
		LogManager::log(LOG_ERROR, "Failed to get vtable from host");
		return;
	}

	// Dump vtable entries (Optional)
	//LogManager::log(LOG_DEBUG, "Dumping vtable entries:");
	//for (int i = 0; i < 10; i++) {
	//	LogManager::log(LOG_DEBUG, "vtable[{}] = {}", i, vtable[i]);
	//}
	//LogManager::log(LOG_DEBUG, "Finished dumping vtable entries");

	// Retrieve the addresses of all required methods
	void* vtable_TrackedDevicePoseUpdated = vtable[Offset_TrackedDevicePoseUpdated];

	// Initialize MinHook
	if (MH_Initialize() != MH_OK) {
		LogManager::log(LOG_ERROR, "MinHook Initialization failed");
		return;
	}

	// Create MinHook hooks
	if (MH_CreateHook(vtable_TrackedDevicePoseUpdated, &overrideTrackedDevicePoseUpdated, reinterpret_cast<void**>(&originalTrackedDevicePoseUpdated))
		!= MH_OK) {
		LogManager::log(LOG_ERROR, "MinHook hook creation failed (TrackedDevicePoseUpdated)");
		return;
	}

	// Enable MinHook hooks
	if (MH_EnableHook(vtable_TrackedDevicePoseUpdated) != MH_OK) {
		LogManager::log(LOG_ERROR, "MinHook hook enable failed (TrackedDevicePoseUpdated)");
		return;
	}

	LogManager::log(LOG_INFO, "All methods hooked successfully");
}

void HookManager::shutdown(std::string reason) {
	MH_Uninitialize();

	LogManager::log(LOG_INFO, "Shutdown called: {}", reason);
	LogManager::shutdown();

	Sleep(1000);

	// Start exiting thread
	CreateThread(nullptr, 0, &HookManager::ejectThread, nullptr, 0, nullptr);
}

DWORD __stdcall HookManager::ejectThread(LPVOID) {
	Sleep(100);
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

void HookManager::setHModule(HMODULE hModule) {
	HookManager::hModule = hModule;
}