
#include "BLDCInterface.h"
#include <unistd.h>

#include <vector>

int main(void) {
	bldcInterface::BLDCInterface iface;

	auto enMotorHandle  = iface.getHandle<uint8_t>("motor.enable", true);
	auto speedHandle    = iface.getHandle<float>("motor.target_frequency", true);
	auto steeringHandle = iface.getHandle<std::array<uint16_t, 2>>("servo.pwm_values", true);
	auto odometryHandle = iface.getHandle<int64_t>("hall.odometry", true);

	auto remotePWMHandle = iface.getRemoteSettableHandle<std::array<uint16_t, 4>>(1, "pwm.manual"); // control the pwm values on device with id 1
	remotePWMHandle = { 1, 2, 3, 4 };

	odometryHandle = 0LL;
	steeringHandle = {1000, 1500};

	enMotorHandle = 1;
	speedHandle  = 50;
	usleep(1000000);

	std::cout << "odometry: " << odometryHandle << std::endl;
	enMotorHandle = 0;
	return 0;
}

