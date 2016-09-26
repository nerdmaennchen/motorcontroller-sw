#pragma once

#include "usb/USBInterface.h"
#include <iostream>
#include <map>

namespace bldcInterface
{

class BLDCInterface final
{
	struct ConfigurationHandleBase {
		uint8_t index;
		uint16_t size;
		std::string name;
		std::string format;
	};

	USBInterface mInterface;
	std::map<std::string, ConfigurationHandleBase> mConfigurations;

	template<typename T>
	class ConfigurationHandle {
	public:
		ConfigurationHandle(ConfigurationHandleBase* handle, BLDCInterface *iface, bool autoSync) : mHandle(handle), mIface(iface), mAutoSync(autoSync) {
			pull();
		}
		ConfigurationHandle(ConfigurationHandleBase* handle, BLDCInterface *iface, T const& value, bool autoSync) : mHandle(handle), mIface(iface), mAutoSync(autoSync), mValue(value) {
			if (mAutoSync) {
				push();
			} else {
				pull();
			}
		}

		void push() {
			mIface->setConfig(mHandle, &mValue);
		}

		void pull() {
			mIface->getConfig(mHandle, &mValue);
		}

		operator T() {
			if (mAutoSync) {
				pull();
			}
			return mValue;
		}

		T* operator->() {
			return &mValue;
		}

		T& operator=(T const& rhs) {
			mValue = rhs;
			if (mAutoSync) {
				push();
			}
			return mValue;
		}

		void setAutopush(bool autopush) {
			mAutoSync = autopush;
		}
	private:
		ConfigurationHandleBase* mHandle;
		BLDCInterface *mIface;
		bool mAutoSync {false};
		T mValue;
	};

public:
	BLDCInterface();
	virtual ~BLDCInterface();

	void setConfig(ConfigurationHandleBase const* handle, void const* values);
	void getConfig(ConfigurationHandleBase const* handle, void* values);

	template<typename T>
	ConfigurationHandle<T> const getHandle(std::string const& name, bool autoSync=false) {
		return ConfigurationHandle<T>(&(mConfigurations[name]), this, autoSync);
	}
	template<typename T>
	ConfigurationHandle<T> const getHandle(std::string const& name, T const& initval, bool autoSync=false) {
		return ConfigurationHandle<T>(&(mConfigurations[name]), this, initval, autoSync);
	}
};

}
