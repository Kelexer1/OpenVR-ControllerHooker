#pragma once
#include <openvr_driver.h>

#include "ControllerTypes.h"

vr::ETrackedDeviceClass getDeviceClass(uint32_t deviceIndex);

ControllerPose FromDriverPose(const vr::DriverPose_t& pose);

vr::DriverPose_t ToDriverPose(const ControllerPose& cp);