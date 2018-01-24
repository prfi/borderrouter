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

#ifndef DBUS_NETWORK_MANAGER_BASE_HPP
#define DBUS_NETWORK_MANAGER_BASE_HPP

#include <iostream>
#include <list>
#include <string>

#include <dbus/dbus.h>

#include "common/logging.hpp"

#include "wifi_controller_common.hpp"

namespace ot {
namespace Dbus {

class DbusNetworkManagerBase {

public:
	DbusNetworkManagerBase(const char *aDest, const char *aPath, const char *aIface);

protected:
	DBusConnection *getConnection();

	DBusMessage *createMessagePropertyGet(const char *propName);
	DBusMessage *createMessageMethodCall(const char *methodName);
	
	std::list<std::string> extractStringsFromObjectPathArray(DBusMessage *msg, DBusError *error);
	
	std::string extractStringFromVariant(DBusMessage *msg, DBusError *error);

	uint32_t extractUInt32FromVariant(DBusMessage *msg, DBusError *error);

	std::string extractObjectPath(DBusMessage *msg, DBusError *error);

protected:
	const char *mDest;
	const char *mPath;
	const char *mIface;
};

void add_bool(DBusMessageIter *iter, const char *str);

void add_string(DBusMessageIter *iter, const char *str);

void add_bytearray(DBusMessageIter *iter, const char *str);

void message_add_dict_entry(DBusMessageIter *iter, void (*func)(DBusMessageIter *, const char *), const char *name, const char *value);

} //namespace Dbus
} //namespace ot
#endif //DBUS_NETWORK_MANAGER_BASE_HPP
