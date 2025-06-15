#include "globals.h"
SharedControllerMemoryIO sharedMemoryIO(nullptr);
ControllerCommandSender commandSender(&sharedMemoryIO);
