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

#include "dbus_network_manager_device.hpp"

namespace ot {
namespace Dbus {

DbusNetworkManagerDevice::DbusNetworkManagerDevice(const char *aPath) :
	DbusNetworkManagerBase("org.freedesktop.NetworkManager",
						   aPath,
						   "org.freedesktop.NetworkManager.Device")
{
}

int DbusNetworkManagerDevice::Interface(std::string &aInterface)
{
	int            ret       = kWifiStatus_Ok;
	DBusConnection *dbusConn = NULL;
	DBusMessage    *queryMsg = NULL;
	DBusMessage     *reply   = NULL;
	DBusError       error;

	dbusConn = getConnection();

	queryMsg = createMessagePropertyGet("Interface");

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, 1000, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);

	aInterface = extractStringFromVariant(reply, &error);

exit:
	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "property \"Interface\" get; error: %s", error.message);
	}

	dbus_error_free(&error);

	if (queryMsg) {
		dbus_message_unref(queryMsg);
	}

	if (reply) {
		dbus_message_unref(reply);
	}

	if (dbusConn) {
		dbus_connection_unref(dbusConn);
	}

	return ret;
}

int DbusNetworkManagerDevice::DeviceType(uint32_t *pDeviceType)
{
 	int            ret       = kWifiStatus_Ok;
 	DBusConnection *dbusConn = NULL;
 	DBusMessage    *queryMsg = NULL;
 	DBusMessage    *reply    = NULL;
 	DBusError      error;

 	 VerifyOrExit(pDeviceType != NULL, ret = kWifiStatus_InvalidArgument);

 	dbusConn = getConnection();

 	queryMsg = createMessagePropertyGet("DeviceType");

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, 1000, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);

	*pDeviceType = extractUInt32FromVariant(reply, &error);
	
exit:
	if (dbus_error_is_set(&error))
	{
		std::cout << "property \"DeviceType\" get; error: %s" << error.message << std::endl;
	}

	dbus_error_free(&error);

	if (queryMsg) {
		dbus_message_unref(queryMsg);
	}

	if (reply) {
		dbus_message_unref(reply);
	}

	if (dbusConn) {
		dbus_connection_unref(dbusConn);
	}

	return ret;
}

} //namespace Dbus
} //namespace ot
