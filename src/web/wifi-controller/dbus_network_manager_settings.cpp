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

#include "dbus_network_manager_settings.hpp"

namespace ot {
namespace Dbus {

static const char *connection_section_name = "connection";
static const char *wireless_section_name = "802-11-wireless";
static const char *wireless_security_section_name = "802-11-wireless-security";
static const char *ipv4_section_name = "ipv4";
static const char *ipv6_section_name = "ipv6";

	
DbusNetworkManagerSettings::DbusNetworkManagerSettings() :
	DbusNetworkManagerBase("org.freedesktop.NetworkManager",
						   "/org/freedesktop/NetworkManager/Settings",
						   "org.freedesktop.NetworkManager.Settings")
{
}

int DbusNetworkManagerSettings::ListConnections(std::list<std::string> &connections)
{
	int            ret       = kWifiStatus_Ok;
	DBusConnection *dbusConn = NULL;
	DBusMessage    *queryMsg = NULL;
	DBusMessage    *reply    = NULL;
	DBusError      error;

	dbusConn = getConnection();

	queryMsg = createMessageMethodCall("ListConnections");

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, 1000, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);
 	
 	connections = extractStringsFromObjectPathArray(reply, &error);

exit:
	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "list connections; error: %s", error.message);
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

int DbusNetworkManagerSettings::AddWirelessConnection(const std::string &ssid,
									                     	  const std::string &password,
									                     	  std::string &newConnPath)
{

	int            ret       = kWifiStatus_Ok;
	DBusConnection *dbusConn = NULL;
	DBusMessage    *queryMsg = NULL;
	DBusMessage    *reply    = NULL;
	DBusError      error;

	dbusConn = getConnection();

	queryMsg = createMessageMethodCall("AddConnection");

	DBusMessageIter iter, section, section_array, values_array;

	dbus_message_iter_init_append(queryMsg, &iter);
	dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sa{sv}}", &section_array);

	// Connection
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &connection_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_string, "id", ssid.c_str());
	message_add_dict_entry(&values_array, add_string, "type", "802-11-wireless");
	message_add_dict_entry(&values_array, add_bool, "autoconnect", "true");

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	// Wireless
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &wireless_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_bytearray, "ssid", ssid.c_str());
	message_add_dict_entry(&values_array, add_string, "mode", "infrastructure");
	message_add_dict_entry(&values_array, add_string, "security", "802-11-wireless-security");

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	// Wireless Security
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &wireless_security_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_string, "auth-alg", "open");
	message_add_dict_entry(&values_array, add_string, "key-mgmt", "wpa-psk");
	message_add_dict_entry(&values_array, add_string, "psk", password.c_str());

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	// IPv4
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &ipv4_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_string, "method", "auto");

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	// IPv6
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &ipv6_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_string, "method", "ignore");

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	dbus_message_iter_close_container(&iter, &section_array);

	// Send message
	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(dbusConn, queryMsg, -1, &error);
	VerifyOrExit(reply != NULL, ret = kWifiStatus_InvalidReply);
	
	newConnPath = extractObjectPath(reply, &error);

exit:
	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "add wireless connection; error: %s", error.message);
	}

	dbus_error_free(&error);

	if (reply) {
		dbus_message_unref(reply);
	}

	if (queryMsg) {
		dbus_message_unref(queryMsg);
	}

	if (dbusConn) {
		dbus_connection_unref(dbusConn);
	}

	return ret;
}

} //namespace Dbus
} //namespace ot
