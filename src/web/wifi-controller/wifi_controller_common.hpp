#ifndef WIFI_CONTROLLER_COMMON_HPP
#define WIFI_CONTROLLER_COMMON_HPP

namespace ot {
namespace Dbus {

enum
{
    kWifiStatus_Ok                     = 0,
    kWifiStatus_Failure                = 1,
    kWifiStatus_InvalidArgument        = 2,
    kWifiStatus_ConnectToNetworkFailed = 3,
    kWifiStatus_GetDevicesFailed       = 4,
    kWifiStatus_NoWifiDevAvailable     = 5,
    kWifiStatus_InvalidConnection      = 6,
    kWifiStatus_InvalidMessage         = 7,
    kWifiStatus_InvalidReply           = 8,
};

} //namespace Dbus
} //namespace ot
#endif //WIFI_CONTROLLER_COMMON_HPP
