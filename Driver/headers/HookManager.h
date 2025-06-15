#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <MinHook.h>
#include <openvr_driver.h>

/**
 * @brief Integer offsets from the IVRServerDriverHost interface vtable.
 * 
 * These offsets correspond to the ordering of the methods as found in <openvr_driver.h> 
 * within the IVRServerDriverHost class. For example, TrackedDevicePoseUpdated() is the
 * second entry under IVRServerDriverHost, so it's index is 1
 */
enum VTableOffsets {
	Offset_TrackedDevicePoseUpdated = 1 // IVRServerDriverHost->TrackedDevicePoseUpdated()
};

/**
 * @brief Manages OpenVR method hooks throughout the driver lifespan
 */
class HookManager {
public:
	/**
	 * @brief Initializes MinHook and hooks to all required methods
	 * @param host A pointer to the IVRServerDriverHost interface
	 */
	static void setupHooks(void* host);

	/**
	 * @brief Gracefully shuts down MinHook in the event of an error
	 */
	static void shutdown(std::string reason);

	/**
	 * @brief Frees the associated module and exits the thread
	 */
	static DWORD __stdcall ejectThread(LPVOID);

	/**
	 * @brief Sets the associated hModule
	 */
	static void setHModule(HMODULE hModule);
private:
	/// The associated hModule
	static HMODULE hModule;
};