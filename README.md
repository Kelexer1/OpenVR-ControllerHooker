## 📌 Brief
OpenVR ControllerHooker is a custom OpenVR driver for SteamVR that intercepts controller pose updates in real-time. It exposes an easy-to-use client-side API via shared memory, allowing external applications to read and modify numerous properties of controllers, such as position, velocity, and rotation.

## 🔧 Installation
1. Clone the repository
	- Either download the code as a .zip, or run the following command:
	- `git clone https://github.com/kelexer1/ControllerHooker.git`
2. Build the project
	- This requires Microsoft Visual Studio 2022 for a fresh build, although you can find prebuild files already included
3. Register the ControllerHooker driver, there are 2 ways to do this
	1. Register the build directory
		1. In a command terminal, navigate to your SteamVR bin directory, usually by
		   `cd C:/Program Files (x86)/Steam/steamapps/common/SteamVR/bin/win64`
		2. Run the following command to register the driver (replace `{installation path}`) with the path to the root folder of the repository
		   `vrpathreg adddriver {installation path}/Build/ControllerHookerDriver/Release`
	2. Copy the driver to .../Steam/steamapps/common/SteamVR/drivers
		1. In a command terminal, navigate to your SteamVR drivers directory, usually by
		   `cd C:/Program Files (x86)/Steam/steamapps/common/SteamVR/drivers`
		2. Make a new folder called `controllerhooker`, and paste the contents of `{installation path}/Build/ControllerHookerDriver/Release` into it (replace `{installation path}` with the path to the root folder of the repository)
		3. In a command terminal, navigate to your SteamVR bin directory, usually by
		   `cd C:/Program Files (x86)/Steam/steamapps/common/SteamVR/bin/win64`
		4. Run the following command to register the driver (replace `{driver path}`) with the the `controllerhooker` folder you created earlier
		   `vrpathreg adddriver {driver path}`

## 💡 Features
- Intercepts and overrides OpenVR controller pose updates
- Options to override poses on command
- Options to communicate with ControllerHooker driver to replace poses on a frame-by-frame basis
- Exposes real-time shared memory interface for client control
- Sample client applications included
- Extensive inline documentation
- Thread-safe, high-frequency safe design

## 🛑 Limitations / Known Bugs
- Unable to intercept or modify inputs like button presses
- Not designed to concurrently run more than 1 client application at a time

## 🪵 Logs and Debugging
Driver logs are written to `C:/temp/log_ControllerHooker`, logging by client applications may vary

## 💻 Using the Client API
- Ensure you have the required files (you can copy paste them from the following directories on the repository root)
	- `SharedFiles/headers`
		- `ControllerCommandSender.h`
		- `ControllerTypes.h`
		- `IControllerCommandSender`
		- `IControllerEventListener`
		- `SharedControllerMemory.h`

		***Third party files***
		- `openvr.h` (from OpenVR SDK)
	- `Build/ControllerHookerLib/{configuration}/ControllerHookerLib.lib`
		- `{configuration}` can be either `Debug` or `Release`, depending on your use
- Link against `ControllerHookerLib.lib` and include the header files listed above
- Consult the sample applications (particularly `MyFirstApplication`) to see how to initialize the application

## 🧪 Sample Applications
- `MyFirstApplication`: Demonstrates initialization and basic pose modification
	- Notice how when statically writing a pose, the controller will continue to use that pose as long as `useCustomPose` is `true`
- `PingPong`: Demonstrates how to use ping-pong communication to provide poses that the driver will use on a frame-by-frame basis. This is particularly useful when you want minimal latency between pose updates, and when you want to work with the natural pose of the controller each frame

## 📚 API Reference
Full class and method documentation is available inline in the header files listed in "Using the Client API"

## 🚀 Future Plans
- Full controller input manipulation system for buttons, triggers, joysticks, etc.

## License
This project is licensed under the MIT license. See the [LICENSE](./LICENSE) file for details