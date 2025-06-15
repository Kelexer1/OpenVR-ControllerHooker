#include "Controller.h"

Controller::Controller(uint32_t deviceIndex, vr::ETrackedControllerRole role) :
	index(deviceIndex), role(role) {
	useCustomPose = false;

	vr::PropertyContainerHandle_t container = vr::VRProperties()->TrackedDeviceToPropertyContainer(deviceIndex);
	serial = vr::VRProperties()->GetStringProperty(container, vr::Prop_SerialNumber_String);

	overridenPose = this->getNaturalPose();
}

Controller::~Controller() {}

void Controller::setPose(const vr::DriverPose_t& newPose) {
	overridenPose = newPose;
}

void Controller::setPosition(const vr::HmdVector3_t& newPosition) {
	overridenPose.vecPosition[0] = newPosition.v[0];
	overridenPose.vecPosition[1] = newPosition.v[1];
	overridenPose.vecPosition[2] = newPosition.v[2];
}

void Controller::setRotation(const vr::HmdQuaternion_t& newRotation) {
	overridenPose.qRotation.w = newRotation.w;
	overridenPose.qRotation.x = newRotation.x;
	overridenPose.qRotation.y = newRotation.y;
	overridenPose.qRotation.z = newRotation.z;
}

void Controller::setVelocity(const vr::HmdVector3_t& newVelocity) {
	overridenPose.vecVelocity[0] = newVelocity.v[0];
	overridenPose.vecVelocity[1] = newVelocity.v[1];
	overridenPose.vecVelocity[2] = newVelocity.v[2];
}

void Controller::setAngularVelocity(const vr::HmdVector3_t& newAngularVelocity) {
	overridenPose.vecAngularVelocity[0] = newAngularVelocity.v[0];
	overridenPose.vecAngularVelocity[1] = newAngularVelocity.v[1];
	overridenPose.vecAngularVelocity[2] = newAngularVelocity.v[2];
}

void Controller::setAcceleration(const vr::HmdVector3_t& newAcceleration) {
	overridenPose.vecAcceleration[0] = newAcceleration.v[0];
	overridenPose.vecAcceleration[1] = newAcceleration.v[1];
	overridenPose.vecAcceleration[2] = newAcceleration.v[2];
}

void Controller::setAngularAcceleration(const vr::HmdVector3_t& newAngularAcceleration) {
	overridenPose.vecAngularAcceleration[0] = newAngularAcceleration.v[0];
	overridenPose.vecAngularAcceleration[0] = newAngularAcceleration.v[0];
	overridenPose.vecAngularAcceleration[0] = newAngularAcceleration.v[0];
}

const vr::DriverPose_t& Controller::getPose() const {
	return overridenPose;
}

const vr::DriverPose_t& Controller::getNaturalPose() const {
	return naturalPose;
}

void Controller::setUseCustomPose(const bool overridePose) {
	this->useCustomPose = overridePose;
}

bool Controller::getUseCustomPose() const {
	return useCustomPose;
}

const uint32_t Controller::getIndex() const {
	return index;
}

const vr::ETrackedControllerRole Controller::getRole() const {
	return role;
}

const std::string Controller::getSerial() const {
	return serial;
}