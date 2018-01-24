/*
 *  Copyright (c) 2018, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "common/code_utils.hpp"

#include "wifi_controller.hpp"

#include "dbus_network_manager.hpp"
#include "dbus_network_manager_device.hpp"
#include "dbus_network_manager_settings.hpp"

namespace ot {
namespace Dbus {

int WifiController::connectToNetwork(const std::string &ssid, const std::string &password)
{
	int      ret        = kWifiStatus_Ok;
	uint32_t deviceType = kNetworkManagerDevType_UNKNOWN;

	DbusNetworkManager 		   nm;
	DbusNetworkManagerSettings nmSettings;

	std::list<std::string>     devices;
	std::list<std::string>     wifiDevices;

	std::string                connection_path; 

	VerifyOrExit(nm.GetDevices(devices) == kWifiStatus_Ok, ret = kWifiStatus_GetDevicesFailed);

	// Filter out devices different than WiFi.
	for (auto device : devices)
	{
		DbusNetworkManagerDevice nmDevice(device.c_str());

		nmDevice.DeviceType(&deviceType);

		if (deviceType == kNetworkManagerDevType_WIFI)
		{
			wifiDevices.push_back(device);
		}
	}

	VerifyOrExit(!wifiDevices.empty(), ret = kWifiStatus_NoWifiDevAvailable);

	// Create wireless conneection
	ret = nmSettings.AddWirelessConnection(ssid.c_str(), 
										   password.c_str(),
										   connection_path);
	VerifyOrExit(ret == kWifiStatus_Ok, ret = kWifiStatus_ConnectToNetworkFailed);

	// Activate connection
	for (auto wifiDev : wifiDevices)
	{
		nm.ActivateConnection(connection_path.c_str(), wifiDev.c_str());
	}

exit:
	return ret;
}

} //namespace Dbus
} //namespace ot
