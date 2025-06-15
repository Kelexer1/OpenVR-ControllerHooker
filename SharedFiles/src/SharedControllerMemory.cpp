#include "SharedControllerMemory.h"

SharedControllerMemoryIO::SharedControllerMemoryIO(IControllerEventListener* eventListener) :
	hMapFile(NULL), sharedMemory(nullptr), hMutex(nullptr), eventListener(eventListener) {}

SharedControllerMemoryIO::~SharedControllerMemoryIO() {
	if (sharedMemory) {
		UnmapViewOfFile(sharedMemory);
	}

	if (hMapFile) {
		CloseHandle(hMapFile);
	}

	if (hMutex) {
		CloseHandle(hMutex);
	}

	pollExitRequested = true;
	if (pollThread.joinable()) {
		pollThread.join();
	}
}

bool SharedControllerMemoryIO::initialize(bool isDriver) {
	if (isDriver) {
		hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
			0, sizeof(SharedControllerMemory), SHM_NAME);
	} else {
		hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
	}

	if (!hMapFile) {
		return false;
	}

	sharedMemory = static_cast<SharedControllerMemory*>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));

	if (!sharedMemory) {
		return false;
	}

	if (isDriver) {
		hMutex = CreateMutexA(nullptr, FALSE, SHM_MUTEX_NAME);
	} else {
		hMutex = OpenMutexA(SYNCHRONIZE, FALSE, SHM_MUTEX_NAME);
	}

	if (!hMutex) {
		return false;
	}

	if (isDriver) {
		if (eventListener) {
			pollThread = std::thread([this]() {
				while (!pollExitRequested.load()) {
					pollForUpdates();
				}
				});
		}
	}

	return true;
}

void SharedControllerMemoryIO::writeOverridenPose(uint32_t deviceIndex, const ControllerPose& pose) {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES) {
		return;
	}
	WaitForSingleObject(hMutex, INFINITE);
	sharedMemory->slots[deviceIndex].overridenPose = pose;
	sharedMemory->slots[deviceIndex].deviceIndex = deviceIndex;
	sharedMemory->slots[deviceIndex].isOccupied = true;

	ReleaseMutex(hMutex);
}

bool SharedControllerMemoryIO::readOverridenPose(uint32_t deviceIndex, ControllerPose& outPose) const {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES || !sharedMemory->slots[deviceIndex].isOccupied) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	outPose = sharedMemory->slots[deviceIndex].overridenPose;
	ReleaseMutex(hMutex);
	return true;
}

void SharedControllerMemoryIO::writeNaturalPose(uint32_t deviceIndex, const ControllerPose& pose) {
	if (!sharedMemory || !hMutex ||  deviceIndex >= MAX_DEVICES) {
		return;
	}
	WaitForSingleObject(hMutex, INFINITE);
	sharedMemory->slots[deviceIndex].naturalPose = pose;
	sharedMemory->slots[deviceIndex].deviceIndex = deviceIndex;
	sharedMemory->slots[deviceIndex].isOccupied = true;

	sharedMemory->slots[deviceIndex].naturalPoseUpdateCounter.fetch_add(1, std::memory_order_release);
	ReleaseMutex(hMutex);
}

bool SharedControllerMemoryIO::readNaturalPose(uint32_t deviceIndex, ControllerPose& outPose) const {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES || !sharedMemory->slots[deviceIndex].isOccupied) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	outPose = sharedMemory->slots[deviceIndex].naturalPose;
	ReleaseMutex(hMutex);
	return true;
}

void SharedControllerMemoryIO::writeUseCustomPose(uint32_t deviceIndex, const bool useCustomPose) {
	if (!sharedMemory || !hMutex ||  deviceIndex >= MAX_DEVICES) {
		return;
	}
	WaitForSingleObject(hMutex, INFINITE);
	sharedMemory->slots[deviceIndex].useCustomPose = useCustomPose;
	sharedMemory->slots[deviceIndex].deviceIndex = deviceIndex;
	sharedMemory->slots[deviceIndex].isOccupied = true;
	ReleaseMutex(hMutex);
}

bool SharedControllerMemoryIO::readUseCustomPose(uint32_t deviceIndex, bool& outUseCustomPose) const {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES || !sharedMemory->slots[deviceIndex].isOccupied) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	outUseCustomPose = sharedMemory->slots[deviceIndex].useCustomPose;
	ReleaseMutex(hMutex);
	return true;
}

bool SharedControllerMemoryIO::isControllerSlotActive(uint32_t deviceIndex, bool& outIsOccupied) const {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	//outIsOccupied = sharedMemory->slots[deviceIndex].isOccupied;
	outIsOccupied = true;
	ReleaseMutex(hMutex);
	return true;
}

bool SharedControllerMemoryIO::readControllerNaturalPoseCounter(uint32_t deviceIndex, int32_t& outCounter) {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES || !sharedMemory->slots[deviceIndex].isOccupied) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	outCounter = sharedMemory->slots[deviceIndex].naturalPoseUpdateCounter;
	ReleaseMutex(hMutex);
	return true;
}

bool SharedControllerMemoryIO::readUsePingPong(uint32_t deviceIndex, bool& outUsePingPing) {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES || !sharedMemory->slots[deviceIndex].isOccupied) {
		return false;
	}
	WaitForSingleObject(hMutex, INFINITE);
	outUsePingPing = sharedMemory->slots[deviceIndex].usePingPong;
	ReleaseMutex(hMutex);
	return true;
}

void SharedControllerMemoryIO::writeUsePingPong(uint32_t deviceIndex, const bool usePingPong) {
	if (!sharedMemory || !hMutex || deviceIndex >= MAX_DEVICES) {
		return;
	}
	WaitForSingleObject(hMutex, INFINITE);
	sharedMemory->slots[deviceIndex].usePingPong = usePingPong;
	sharedMemory->slots[deviceIndex].deviceIndex = deviceIndex;
	sharedMemory->slots[deviceIndex].isOccupied = true;
	ReleaseMutex(hMutex);
}

SharedControllerMemory* SharedControllerMemoryIO::getSharedControllerMemory() const {
	return sharedMemory;
}

// Driver side
ControllerPose SharedControllerMemoryIO::pingPong(uint32_t deviceIndex) {
	auto& slot = sharedMemory->slots[deviceIndex];

	slot.requestPoseUpdate.store(true, std::memory_order_release);
	slot.respondPoseUpdate.store(false, std::memory_order_release);

	constexpr int maxWaitMs = 15;
	for (int i = 0; i < maxWaitMs; i++) {
		if (slot.respondPoseUpdate.load(std::memory_order_acquire)) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return slot.respondPoseUpdate ? slot.overridenPose : slot.naturalPose;
}

// Client side
void SharedControllerMemoryIO::pollForUpdates() {
	for (uint32_t i = 0; i < MAX_DEVICES; i++) {
		bool usePingPong;
		if (readUsePingPong(i, usePingPong) && usePingPong) {
			auto& slot = sharedMemory->slots[i];

			if (slot.requestPoseUpdate.load(std::memory_order_acquire)) {
				WaitForSingleObject(hMutex, INFINITE);
				const ControllerPose naturalPose = slot.naturalPose;

				ControllerPose newPose = eventListener->poseRequested(i, naturalPose);

				slot.overridenPose = newPose;
				slot.requestPoseUpdate.store(false, std::memory_order_release);
				slot.respondPoseUpdate.store(true, std::memory_order_release);

				ReleaseMutex(hMutex);
			}
		}
	}
}