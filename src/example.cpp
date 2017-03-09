
#include "BLDCInterface.h"
#include <unistd.h>

#include <vector>

int main(void) {
	bldcInterface::BLDCInterface iface;
	int remoteDevice = 4;
	auto remotePWMHandle    = iface.getRemoteSettableHandle<std::array<uint16_t, 4>>(remoteDevice, "pwm.manual"); // control the pwm values on device with id 1
	auto remoteEnableHandle = iface.getRemoteSettableHandle<uint8_t>(remoteDevice, "pwm.drv.enable"); // enable the pwm driver device with id 1

	for (int i = 0; i < 1; ++i) {
		remoteEnableHandle = 1;
		usleep(1000);
		remotePWMHandle = { 22, 50, 0, 123 };
		usleep(1000000);
		remotePWMHandle = { 0, 0, 0, 123 };
	}
	return 0;
}

