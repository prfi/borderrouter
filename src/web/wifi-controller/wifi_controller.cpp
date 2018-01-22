#include <iostream>
#include <cstring>
#include <list>

#include <dbus/dbus.h>

static const int bool_true = 1;
static const int bool_false = 0;

static const char *connection_section_name = "connection";
static const char *wireless_section_name = "802-11-wireless";
static const char *wireless_security_section_name = "802-11-wireless-security";
static const char *ipv4_section_name = "ipv4";
static const char *ipv6_section_name = "ipv6";


static inline void add_bool(DBusMessageIter *msg_iter, const char *str)
{
	const int *p_value = NULL;

	if (strcmp(str, "true") == 0)
	{
		p_value = &bool_true;
	}
	else if (strcmp(str, "false") == 0)
	{
		p_value = &bool_false;
	}

	if (!p_value)
	{
		return;
	}

	DBusMessageIter var;

	dbus_message_iter_open_container(msg_iter, DBUS_TYPE_VARIANT, "b", &var);

	dbus_message_iter_append_basic(&var, DBUS_TYPE_BOOLEAN, p_value);

	dbus_message_iter_close_container(msg_iter, &var);
}


static inline void add_string(DBusMessageIter *msg_iter, const char *str)
{
	DBusMessageIter var;

	dbus_message_iter_open_container(msg_iter, DBUS_TYPE_VARIANT, "s", &var);

	dbus_message_iter_append_basic(&var, DBUS_TYPE_STRING, &str);

	dbus_message_iter_close_container(msg_iter, &var);
}


static inline void add_bytearray(DBusMessageIter *msg_iter, const char *str)
{
	DBusMessageIter array, var;

	dbus_message_iter_open_container(msg_iter, DBUS_TYPE_VARIANT, "ay", &var);

	dbus_message_iter_open_container(&var, DBUS_TYPE_ARRAY, "y", &array);

	for (int i = 0; i < strlen(str); i++)
	{
		dbus_message_iter_append_basic(&array, DBUS_TYPE_BYTE, &str[i]);
	}

	dbus_message_iter_close_container(&var, &array);

	dbus_message_iter_close_container(msg_iter, &var);
}


static void message_add_dict_entry(DBusMessageIter *msg_iter, void (*func)(DBusMessageIter *, const char *), const char *name, const char *value)
{
	DBusMessageIter entry;

	dbus_message_iter_open_container(msg_iter, DBUS_TYPE_DICT_ENTRY, NULL, &entry);

	dbus_message_iter_append_basic(&entry, DBUS_TYPE_STRING, &name);

	func(&entry, value);
	
	dbus_message_iter_close_container(msg_iter, &entry);
}


DBusConnection *getConnection(void)
{
	DBusError error;
	DBusConnection *connection;

	dbus_error_init(&error);

	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	if (!connection || dbus_error_is_set(&error))
	{
		std::cout << "ERROR:: " << error.message << std::endl;
	}

	dbus_error_free(&error);

	return connection;
}

// int main(int argc, char **argv)
// {
// 	// std::cout << get_devices().size() << std::endl;
// 	create_wireless_connection("Linksys", "nordicBR");
//
// 	return 0;
// }


//dbus-send --system --print-reply 
//connection;;; --dest=org.freedesktop.NetworkManager 
//object;;; org/freedesktop/NetworkManager 
//method;;; org.freedesktop.DBus.Properties.Set

std::list<std::string> get_devices()
{
	std::list<std::string> devices;

	DBusError error;
	DBusConnection *connection;

	DBusMessage *msg = dbus_message_new_method_call("org.freedesktop.NetworkManager",
													"/org/freedesktop/NetworkManager",
													"org.freedesktop.NetworkManager",
													"GetDevices");
	if (!msg) 
	{
		std::cout << "Could not create new method call!" << std::endl;
	}

	connection = getConnection();

	dbus_error_init(&error);

	DBusMessage *reply = dbus_connection_send_with_reply_and_block(connection, msg, -1, &error);
	if (dbus_error_is_set(&error))
	{
		std::cout << "ERROR:: " <<  error.message << std::endl;
	}
 	
 	dbus_error_free(&error);

 	std::cout << "mamam: " << reply << std::endl;

 	DBusMessageIter msg_iter;

 	dbus_message_iter_init(reply, &msg_iter);

 	int current_type;

 	while ((current_type = dbus_message_iter_get_arg_type(&msg_iter)) != DBUS_TYPE_INVALID)
 	{
 		int arg_type;
 		DBusMessageIter array_iter;

 		std::cout << "type: " << current_type << std::endl;

 		dbus_message_iter_recurse(&msg_iter, &array_iter);

 		while ((arg_type = dbus_message_iter_get_arg_type(&array_iter)) != DBUS_TYPE_INVALID)
 		{
 			DBusBasicValue value;

 			dbus_message_iter_get_basic(&array_iter, &value);

 			devices.push_back(std::string(value.str));

 			dbus_message_iter_next(&array_iter);
 		}

 		dbus_message_iter_next(&msg_iter);
 	}

	return devices;
}

void create_wireless_connection(const char *ssid, const char *password)
{
	DBusError error;
	DBusConnection *connection = NULL;

	DBusMessage *reply = NULL;
	DBusMessage *msg = dbus_message_new_method_call("org.freedesktop.NetworkManager",
												   "/org/freedesktop/NetworkManager/Settings",
												   "org.freedesktop.NetworkManager.Settings", 
												   "AddConnection");
	if (!msg) 
	{
		std::cout << "Could not create new method call!" << std::endl;
		goto clean_up;
	}

	DBusMessageIter msg_iter, section, section_array, values_array;

	dbus_message_iter_init_append(msg, &msg_iter);
	dbus_message_iter_open_container(&msg_iter, DBUS_TYPE_ARRAY, "{sa{sv}}", &section_array);

	// Connection
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &connection_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_string, "id", ssid);
	message_add_dict_entry(&values_array, add_string, "type", "802-11-wireless");
	message_add_dict_entry(&values_array, add_bool, "autoconnect", "true");

	dbus_message_iter_close_container(&section, &values_array);
	dbus_message_iter_close_container(&section_array, &section);

	// Wireless
	dbus_message_iter_open_container(&section_array, DBUS_TYPE_DICT_ENTRY, NULL, &section);
	dbus_message_iter_append_basic(&section, DBUS_TYPE_STRING, &wireless_section_name);

	dbus_message_iter_open_container(&section, DBUS_TYPE_ARRAY, "{sv}", &values_array);

	message_add_dict_entry(&values_array, add_bytearray, "ssid", ssid);
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
	message_add_dict_entry(&values_array, add_string, "psk", password);

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

	dbus_message_iter_close_container(&msg_iter, &section_array);

	// Send message
	connection = getConnection();

	dbus_error_init(&error);

	reply = dbus_connection_send_with_reply_and_block(connection, msg, -1, &error);
	if (dbus_error_is_set(&error))
	{
		std::cout << "ERROR:: " <<  error.message << std::endl;
	}

	std::cout << reply << std::endl;

	dbus_error_free(&error);

clean_up:
	if (reply)
	{
		dbus_message_unref(reply);
	}

	if (msg)
	{
		dbus_message_unref(msg);
	}

	if (connection)
	{
		dbus_connection_unref(connection);
	}
}