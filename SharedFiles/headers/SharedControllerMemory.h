#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <thread>

#include "ControllerTypes.h"
#include "IControllerEventListener.h"

constexpr const char* SHM_NAME = "Local\\ControllerHooker_Buffer";
constexpr const char* SHM_MUTEX_NAME = "Local\\ControllerHooker_Buffer_Mutex";
constexpr size_t MAX_DEVICES = 8;

/** 
 * @brief Represents a single controllers state 
 */
struct SharedControllerSlot {
	ControllerPose overridenPose;
	ControllerPose naturalPose;

	uint32_t deviceIndex;

	bool useCustomPose;

	bool usePingPong = false;
	std::atomic<bool> requestPoseUpdate;
	std::atomic<bool> respondPoseUpdate;

	bool isOccupied;

	std::atomic<int32_t> naturalPoseUpdateCounter;
};

/**
 * @brief Represents the shared memory for all controllers
 * 
 * Uses array indexing corresponding to device index
 */
struct SharedControllerMemory {
	SharedControllerSlot slots[MAX_DEVICES];
};

/**
 * @brief Manages reading and writing the shared memory
 * 
 * Thread safe
 */
class SharedControllerMemoryIO {
public:
	SharedControllerMemoryIO(IControllerEventListener* eventListener);
	~SharedControllerMemoryIO();

	/**
	 * @brief Initializes the shared memory buffer
	 * 
	 * This method must be called before any read/write operations occur
	 * 
	 * @param isDriver Whether the current application is the driver, should NOT be true for any client applications
	 * @return True if initialization succeeded, false if it failed
	 */
	bool initialize(bool isDriver = false);

	/**
	 * @brief Writes the overriden pose for a controller
	 * @param deviceIndex The index of the controller
	 * @param pose The new pose for the controller
	 */
	void writeOverridenPose(uint32_t deviceIndex, const ControllerPose& pose);

	/**
	 * @brief Reads the overriden pose for a controller
	 * @param deviceIndex The index of the controller
	 * @param outPose The pose object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool readOverridenPose(uint32_t deviceIndex, ControllerPose& outPose) const;

	/**
	 * @brief Writes the natural pose for a controller
	 * 
	 * This method should not be called by the client, it will be automatically updated by the driver
	 * 
	 * @param deviceIndex The index of the controller
	 * @param pose The new pose for the controller
	 */
	void writeNaturalPose(uint32_t deviceIndex, const ControllerPose& pose);

	/**
	 * @brief Reads the natural pose for a controller
	 * @param deviceIndex The index of the controller
	 * @param outPose The pose object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool readNaturalPose(uint32_t deviceIndex, ControllerPose& outPose) const;

	/**
	 * @brief Writes whether a controller should use its overriden or natural pose
	 * @param deviceIndex The index of the controller
	 * @param useCustomPose True if the controller should use its overriden pose, false if it should use its natural pose
	 */
	void writeUseCustomPose(uint32_t deviceIndex, const bool useCustomPose);

	/**
	 * @brief Reads whether a controller should use its overriden or natural pose
	 * @param deviceIndex The index of the controller
	 * @param outUseCustomPose The bool object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool readUseCustomPose(uint32_t deviceIndex, bool& outUseCustomPose) const;

	/**
	 * @brief Reads whether a controller shared memory slot is active
	 * @param deviceIndex The index of the controller
	 * @param outIsOccupied The bool object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool isControllerSlotActive(uint32_t deviceIndex, bool& outIsOccupied) const;

	/**
	 * @brief Reads the natural pose update count for a controller
	 * @param deviceIndex The index of the controller
	 * @param outCounter The int32_t object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool readControllerNaturalPoseCounter(uint32_t deviceIndex, int32_t& outCounter);

	/**
	 * @brief Reads whether a controller should use a ping-pong style pose update system
	 * @param deviceIndex The index of the controller
	 * @param outUsePingPong The bool object that should be filled with the output
	 * @return True if the operation succeeded, false if it failed
	 */
	bool readUsePingPong(uint32_t deviceIndex, bool& outUsePingPong);

	/**
	 * @brief Writes whether a controller should use ping-pong communication to override poses
	 * @param deviceIndex The index of the controller
	 * @param usePingPong True if the controller should use ping-pong communication, false if it should not
	 */
	void writeUsePingPong(uint32_t deviceIndex, const bool usePingPong);

	/**
	 * @brief Manages a ping-pong round of communication between driver and client
	 * 
	 * The driver requests a pose update, and waits until the client provides one to apply it
	 * This method should not be called by the client
	 */
	ControllerPose pingPong(uint32_t deviceIndex);

	/**
	 * @brief Checks for any relavent updates and notifies the event listener
	 */
	void pollForUpdates();

	/**
	 * @brief Returns a pointer to the raw shared memory
	 * 
	 * This method should not be used unless nesessary, it is not thread safe
	 * 
	 * @return A pointer to the raw shared memory
	 */
	SharedControllerMemory* getSharedControllerMemory() const;

private:
	/// The shared memory handle
	HANDLE hMapFile;

	/// The mutex handle
	HANDLE hMutex;

	/// A pointer to the shared memory struct
	SharedControllerMemory* sharedMemory;

	/// The event reciever that should be notified when required
	IControllerEventListener* eventListener;

	/// The thread that checks for updates
	std::thread pollThread;

	/// A flag for signalling that the polling thread should exit
	std::atomic<bool> pollExitRequested;
};