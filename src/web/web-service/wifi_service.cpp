/*
 *  Copyright (c) 2017, The OpenThread Authors.
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

/**
 * @file
 *   This file implements the wpan controller service
 */

#include "wifi_service.hpp"

#include "common/code_utils.hpp"

namespace ot {
namespace Web {

std::string WifiService::HandleConnectWifiNetworkRequest(const std::string &aConnectWifiNetworkRequest)
{
    Json::Value              root;
    Json::Reader             reader;
    Json::FastWriter         jsonWriter;
    std::string              response;
    std::string              ssid;
    std::string              password;
    int                      ret = ot::Dbus::kWpantundStatus_Ok;   

    VerifyOrExit(reader.parse(aJoinRequest.c_str(), root) == true, ret = kWpanStatus_ParseRequestFailed);

    ssid = root["ssid"].asString();
    password = root["password"].asString();

    create_wireless_connection(ssid.c_str(), password.c_str());

    root.clear();
    root["result"] = mResponseSuccess;

    root["error"] = ret;
    if (ret != ot::Dbus::kWpantundStatus_Ok)
    {
        otbrLog(OTBR_LOG_ERR, "wpan service error: %d", ret);
        root["result"] = mResponseFail;
    }

    response = jsonWriter.write(root);
    return response;
}

} //namespace Web
} //namespace ot


//     char extPanId[OT_EXTENDED_PANID_LENGTH * 2 + 1];

//     Json::Value              root;
//     Json::Reader             reader;
//     Json::FastWriter         jsonWriter;
//     std::string              response;
//     int                      index;
//     std::string              networkKey;
//     std::string              prefix;
//     bool                     defaultRoute;
//     int                      ret = ot::Dbus::kWpantundStatus_Ok;
//     ot::Dbus::WPANController wpanController;

//     VerifyOrExit(reader.parse(aJoinRequest.c_str(), root) == true, ret = kWpanStatus_ParseRequestFailed);
//     index = root["index"].asUInt();
//     networkKey = root["networkKey"].asString();
//     prefix = root["prefix"].asString();
//     defaultRoute = root["defaultRoute"].asBool();

//     wpanController.SetInterfaceName(mIfName);
//     VerifyOrExit(wpanController.Leave() == ot::Dbus::kWpantundStatus_Ok,
//                  ret = ot::Dbus::kWpantundStatus_LeaveFailed);
//     VerifyOrExit(wpanController.Set(kPropertyType_Data,
//                                     "NetworkKey",
//                                     networkKey.c_str()) == ot::Dbus::kWpantundStatus_Ok,
//                  ret = ot::Dbus::kWpantundStatus_SetFailed);
//     VerifyOrExit(wpanController.Join(mNetworks[index].mNetworkName,
//                                      mNetworks[index].mChannel,
//                                      mNetworks[index].mExtPanId,
//                                      mNetworks[index].mPanId) == ot::Dbus::kWpantundStatus_Ok,
//                  ret = ot::Dbus::kWpantundStatus_JoinFailed);
//     VerifyOrExit(wpanController.AddGateway(prefix.c_str(), defaultRoute) == ot::Dbus::kWpantundStatus_Ok,
//                  ret = ot::Dbus::kWpantundStatus_SetGatewayFailed);

//     ot::Utils::Long2Hex(mNetworks[index].mExtPanId, extPanId);
// exit:

//     root.clear();
//     root["result"] = mResponseSuccess;

//     root["error"] = ret;
//     if (ret != ot::Dbus::kWpantundStatus_Ok)
//     {
//         otbrLog(OTBR_LOG_ERR, "wpan service error: %d", ret);
//         root["result"] = mResponseFail;
//     }
//     response = jsonWriter.write(root);
//     return response;