#pragma once

/// These struct declarations are taken directly from the OpenVR SDK, for proper documentation on each structs members, 
/// consult the OpenVR SDK documentation for the respective type

/// struct HmdVector3_t
struct ControllerVector3 {
	float v[3];
};

/// struct HmdQuaternion_t
struct ControllerQuaternion {
	double w, x, y, z;
};

/// enum ETrackingResult
enum ControllerTrackingResult {
	TrackingResult_Uninitialized = 1,

	TrackingResult_Calibrating_InProgress = 100,
	TrackingResult_Calibrating_OutOfRange = 101,

	TrackingResult_Running_OK = 200,
	TrackingResult_Running_OutOfRange = 201,

	TrackingResult_Fallback_RotationOnly = 300,
};

/// struct DriverPose_t
struct ControllerPose {
	double poseTimeOffset;

	ControllerQuaternion qWorldFromDriverRotation;
	double vecWorldFromDriverTranslation[3];

	ControllerQuaternion qDriverFromHeadRotation;
	double vecDriverFromHeadTranslation[3];

	double vecPosition[3];

	double vecVelocity[3];

	double vecAcceleration[3];

	ControllerQuaternion qRotation;

	double vecAngularVelocity[3];

	double vecAngularAcceleration[3];

	ControllerTrackingResult result;

	bool poseIsValid;
	bool willDriftInYaw;
	bool shouldApplyHeadModel;
	bool deviceIsConnected;
};

