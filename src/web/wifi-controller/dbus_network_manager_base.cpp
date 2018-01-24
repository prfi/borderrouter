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

#include "dbus_network_manager_base.hpp"

#include <cstring>

namespace ot {
namespace Dbus {

DbusNetworkManagerBase::DbusNetworkManagerBase(const char *aDest, 
											   const char *aPath,
											   const char *aIface) :
	mDest(aDest),
	mPath(aPath),
	mIface(aIface)
{
}

DBusConnection *DbusNetworkManagerBase::getConnection()
{
	DBusConnection *connection = NULL;
	DBusError 	   error;

	dbus_error_init(&error);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if (dbus_error_is_set(&error))
	{
		otbrLog(OTBR_LOG_ERR, "connection error: %s", error.message);
	}

	dbus_error_free(&error);

	return connection;
}

DBusMessage *DbusNetworkManagerBase::createMessagePropertyGet(const char *propName)
{
	DBusMessage *queryMsg = NULL;

	queryMsg = dbus_message_new_method_call(mDest,
										    mPath, 
						   					"org.freedesktop.DBus.Properties",
						   					"Get");

	dbus_message_append_args(queryMsg,
					     	 DBUS_TYPE_STRING, &mIface,
						 	 DBUS_TYPE_STRING, &propName,
						 	 DBUS_TYPE_INVALID);

	return queryMsg;
}

DBusMessage *DbusNetworkManagerBase::createMessageMethodCall(const char *methodName)
{
	return dbus_message_new_method_call(mDest,
										mPath,
										mIface,
										methodName);
}

std::list<std::string> DbusNetworkManagerBase::extractStringsFromObjectPathArray(DBusMessage *msg,
																				 DBusError *error)
{
	std::list<std::string> result;
	
	DBusMessageIter iter, sub;

	dbus_message_iter_init(msg, &iter);

	if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&iter))
	{
		dbus_set_error_const(error, "reply_should_be_array", "This message hasn't a array response type");
		return result;
	}

	dbus_message_iter_recurse(&iter, &sub);

	while (dbus_message_iter_get_arg_type(&sub) == DBUS_TYPE_OBJECT_PATH)
	{
		DBusBasicValue value;
		dbus_message_iter_get_basic(&sub, &value);

		result.push_back(std::string(value.str));

		(void) dbus_message_iter_next(&sub);
	}

	return result;
}

std::string DbusNetworkManagerBase::extractStringFromVariant(DBusMessage *msg, DBusError *error)
{
	DBusBasicValue value;
	DBusMessageIter iter, sub;

	std::string result;

	dbus_message_iter_init(msg, &iter);

	if (DBUS_TYPE_VARIANT != dbus_message_iter_get_arg_type(&iter)) {
		dbus_set_error_const(error, "reply_should_be_variant", "This message hasn't a variant response type");
		return result;
	}

	dbus_message_iter_recurse(&iter, &sub);

	if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&sub)) {
		dbus_set_error_const(error, "variant_should_be_string", "This variant reply message must have string content");
		return result;
	}

	dbus_message_iter_get_basic(&sub, &value);
	result = value.str;

	return result;
}

uint32_t DbusNetworkManagerBase::extractUInt32FromVariant(DBusMessage *msg, DBusError *error)
{
	DBusBasicValue value;
	DBusMessageIter iter, sub;

	dbus_message_iter_init(msg, &iter);

	if (DBUS_TYPE_VARIANT != dbus_message_iter_get_arg_type(&iter)) {
		dbus_set_error_const(error, "reply_should_be_variant", "This message hasn't a variant response type");
		return 0xffffffff;
	}

	dbus_message_iter_recurse(&iter, &sub);

	if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&sub)) {
		dbus_set_error_const(error, "variant_should_be_uint32_t", "This variant reply message must have uint32_t content");
		return 0xffffffff;
	}

	dbus_message_iter_get_basic(&sub, &value);
	return value.u32;
}

std::string DbusNetworkManagerBase::extractObjectPath(DBusMessage *msg, DBusError *error)
{
	DBusBasicValue value;
	DBusMessageIter iter;

	std::string objectPath;

	dbus_message_iter_init(msg, &iter);

	if (DBUS_TYPE_OBJECT_PATH != dbus_message_iter_get_arg_type(&iter)) {
		dbus_set_error_const(error, "reply_should_be_object_path", "This message hasn't a object path response type");
		return objectPath;
	}
	
	dbus_message_iter_get_basic(&iter, &value);
	objectPath = value.str;

	return objectPath;
}

// Maybe those things below should be put to a some utils file? 
void add_bool(DBusMessageIter *iter, const char *str)
{
	int value = 0;
	const int *p_value = &value;

	if (strcmp(str, "true") == 0)
	{
		value = 1;
	}
	else if (strcmp(str, "false") == 0)
	{
		value = 0;
	}
	else
	{
		otbrLog(OTBR_LOG_ERR, "Could not add bool value to message.");
		return;
	}

	DBusMessageIter var;

	dbus_message_iter_open_container(iter, DBUS_TYPE_VARIANT, "b", &var);
	dbus_message_iter_append_basic(&var, DBUS_TYPE_BOOLEAN, p_value);
	dbus_message_iter_close_container(iter, &var);
}

void add_string(DBusMessageIter *iter, const char *str)
{
	DBusMessageIter var;

	dbus_message_iter_open_container(iter, DBUS_TYPE_VARIANT, "s", &var);
	dbus_message_iter_append_basic(&var, DBUS_TYPE_STRING, &str);
	dbus_message_iter_close_container(iter, &var);
}

void add_bytearray(DBusMessageIter *iter, const char *str)
{
	DBusMessageIter array, var;

	dbus_message_iter_open_container(iter, DBUS_TYPE_VARIANT, "ay", &var);
	dbus_message_iter_open_container(&var, DBUS_TYPE_ARRAY, "y", &array);

	for (unsigned int i = 0; i < strlen(str); i++)
	{
		dbus_message_iter_append_basic(&array, DBUS_TYPE_BYTE, &str[i]);
	}

	dbus_message_iter_close_container(&var, &array);
	dbus_message_iter_close_container(iter, &var);
}


void message_add_dict_entry(DBusMessageIter *iter, void (*func)(DBusMessageIter *, const char *), const char *name, const char *value)
{
	DBusMessageIter entry;

	dbus_message_iter_open_container(iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry);
	dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &name);

	func(&entry, value);
	
	dbus_message_iter_close_container(iter, &entry);
}

} //namespace Dbus
} //namespace ot
