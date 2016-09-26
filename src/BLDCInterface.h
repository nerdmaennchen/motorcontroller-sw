#pragma once

#include "usb/USBInterface.h"
#include <iostream>
#include <istream>
#include <map>
#include <cxxabi.h>

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

public:
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
		ConfigurationHandleBase* handle = &(mConfigurations[name]);
		if (handle->size != sizeof(T)) {
			std::string demangledName = demangle(typeid(T).name());
			std::cerr << "requesting a mapping of " << demangledName << " to " << handle->name << "of incompatible size! sizeof(" << demangledName << ")=" << sizeof(T) << " vs. " << handle->size << std::endl;
			throw "Invalid Configuration Requested";
		}
		return ConfigurationHandle<T>(handle, this, autoSync);
	}
	template<typename T>
	ConfigurationHandle<T> const getHandle(std::string const& name, T const& initval, bool autoSync=false) {
		ConfigurationHandleBase* handle = &(mConfigurations[name]);
		if (handle->size != sizeof(T)) {
			std::string demangledName = demangle(typeid(T).name());
			std::cerr << "requesting a mapping of " << demangledName << " to " << handle->name << "of incompatible size! sizeof(" << demangledName << ")=" << sizeof(T) << " vs. " << handle->size << std::endl;
			throw "Invalid Configuration Requested";
		}
		return ConfigurationHandle<T>(handle, this, initval, autoSync);
	}

private:
	std::string demangle(const char* mangledName) {
	    int status;
	    char* result = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);
	    switch(status) {
	    case -1:
	        std::cerr << "Out of memory!" << std::endl;
	        exit(1);
	    case -2:
	        return mangledName;
	    case -3: // Should never happen, but just in case?
	        return mangledName;
	    }
	    std::string name = result;
	    free(result);
	    return name;
	}
};

}
