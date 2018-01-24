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

#include "dbus_network_manager.hpp"

namespace ot {
namespace Dbus {

DbusNetworkManager::DbusNetworkManager() :
	DbusNetworkManagerBase("org.freedesktop.NetworkManager",
						   "/org/freedesktop/NetworkManager",
						   "org.freedesktop.NetworkManager")
{
}

int DbusNetworkManager::GetDevices(std::list<std::string> &devices)
{
    int            ret       = kWifiStatus_Ok;
	DBusConnection *dbusConn = NULL;
	DBusMessage    *queryMsg = NULL;
	DBusMessage    *reply    = NULL;
	DBusError      error;

	dbusConn = getConnection();

	queryMsg = createMessageMethodCall("GetDevices");

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, 1000, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);
 	
	devices = extractStringsFromObjectPathArray(reply, &error);

 exit:
 	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "get devices; error: %s", error.message);
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

int DbusNetworkManager::ActivateConnection(const char *connection_path, const char *device, const char *specific_object)
{
    int            ret       = kWifiStatus_Ok;
	DBusConnection *dbusConn = NULL;
	DBusMessage    *queryMsg = NULL;
	DBusMessage    *reply    = NULL;
	DBusError      error;

	dbusConn = getConnection();

	queryMsg = createMessageMethodCall("ActivateConnection");

	dbus_message_append_args(queryMsg,
					     	 DBUS_TYPE_OBJECT_PATH, &connection_path,
						 	 DBUS_TYPE_OBJECT_PATH, &device,
						 	 DBUS_TYPE_OBJECT_PATH, &specific_object,
						 	 DBUS_TYPE_INVALID);

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, 1000, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);

 exit:
 	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "activate connection; error: %s", error.message);
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
