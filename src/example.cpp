
#include "BLDCInterface.h"
#include <unistd.h>


int main(void) {
	bldcInterface::BLDCInterface iface;

	auto enMotorHandle  = iface.getHandle<uint8_t>("enable_motor", true);
	auto enBldcHandle   = iface.getHandle<uint8_t>("bldc_enable", true);
	auto speedHandle    = iface.getHandle<int32_t>("bldc_target_frequency", true);
	auto steeringHandle = iface.getHandle<std::array<uint16_t, 2>>("servo_pwm_values", false);

	struct SpeedControllPID {
		float pErr {0};
		float iErr {0};
		float dErr {0};

		float controllP {0};
		float controllI {0};
		float controllD {0};
	};

	auto pidHandle      = iface.getHandle<SpeedControllPID>("bldc_controller");
	// set the pid values
	pidHandle->controllP = 2.5e-4f;
	pidHandle->controllI = 2.e-2f;
	pidHandle->controllD = 2.6e-4f;
	pidHandle.push();

	steeringHandle = {1, 2};
	steeringHandle.push();

	enMotorHandle = 1;
	enBldcHandle  = 1;

	speedHandle  = 100000;
	usleep(1000000);
	speedHandle  = -100000;
	usleep(1000000);

	enMotorHandle = 0;
	enBldcHandle  = 0;

	return 0;
}
