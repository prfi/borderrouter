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

#ifndef DBUS_NETWORK_MANAGER_DEVICE_HPP
#define DBUS_NETWORK_MANAGER_DEVICE_HPP

#include <stdint.h>

#include "dbus_network_manager_base.hpp"

namespace ot {
namespace Dbus {

enum {
	kNetworkManagerDevType_UNKNOWN       = 0,
	kNetworkManagerDevType_ETHERNET      = 1,
	kNetworkManagerDevType_WIFI          = 2,
	kNetworkManagerDevType_UNUSED1       = 3,
	kNetworkManagerDevType_UNUSED2       = 4,
	kNetworkManagerDevType_BT            = 5,
	kNetworkManagerDevType_OLPC_MESH     = 6,
	kNetworkManagerDevType_WIMAX         = 7,
	kNetworkManagerDevType_MODEM         = 8,
	kNetworkManagerDevType_INFINIBAND    = 9,
	kNetworkManagerDevType_BOND          = 10,
	kNetworkManagerDevType_VLAN          = 11,
	kNetworkManagerDevType_ADSL          = 12,
	kNetworkManagerDevType_BRIDGE        = 13,
	kNetworkManagerDevType_GENERIC       = 14,
	kNetworkManagerDevType_TEAM          = 15,
	kNetworkManagerDevType_TUN           = 16,
	kNetworkManagerDevType_IP_TUNNEL     = 17,
	kNetworkManagerDevType_MACVLAN       = 18,
	kNetworkManagerDevType_VXLAN         = 19,
	kNetworkManagerDevType_VETH          = 20,
	kNetworkManagerDevType_MACSEC        = 21,
	kNetworkManagerDevType_DUMMY         = 22,
	kNetworkManagerDevType_PPP           = 23,
	kNetworkManagerDevType_OVS_INTERFACE = 24,
	kNetworkManagerDevType_OVS_PORT      = 25,
	kNetworkManagerDevType_OVS_BRIDGE    = 26
};

class DbusNetworkManagerDevice : public DbusNetworkManagerBase {

public:
	DbusNetworkManagerDevice(const char *aPath);

	int Interface(std::string &aInterface);
	int DeviceType(uint32_t *pDeviceType);
};

} //namespace Dbus
} //namespace ot
#endif //DBUS_NETWORK_MANAGER_DEVICE_HPP
