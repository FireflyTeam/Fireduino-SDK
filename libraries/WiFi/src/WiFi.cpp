 /**
 * @file WiFi.cpp
 * @brief Definitions WiFi class for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include "utility/wifi_drv.h"
#include "WiFi.h"

extern "C" {
  #include "utility/wl_definitions.h"
  #include "utility/wl_types.h"
  #include "utility/debug.h"
}

WiFiClass::WiFiClass()
{
}

void WiFiClass::init()
{
    WiFiDrv::wifiDriverInit();
}

char* WiFiClass::firmwareVersion()
{
	return WiFiDrv::getFwVersion();
}
int WiFiClass::begin(void)
{
	 uint8_t status = WL_IDLE_STATUS;
	 uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;
	
	init();
	if (WiFiDrv::wifiSetNetwork() != WL_FAILURE)
	{
		do
		{
			delay(WL_DELAY_START_CONNECTION);
			status = WiFiDrv::getConnectionStatus();
		}
		while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
	}else
	{
		status = WL_CONNECT_FAILED;
	}
	return status;

}

int WiFiClass::begin(char* ssid)
{
	uint8_t status = WL_IDLE_STATUS;
	uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;

   init();
   if (WiFiDrv::wifiSetNetwork(ssid, strlen(ssid)) != WL_FAILURE)
   {
	   do
	   {
		   delay(WL_DELAY_START_CONNECTION);
		   status = WiFiDrv::getConnectionStatus();
	   }
	   while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
   }else
   {
	   status = WL_CONNECT_FAILED;
   }
   return status;
}

int WiFiClass::begin(char* ssid, uint8_t key_idx, const char *key)
{
	uint8_t status = WL_IDLE_STATUS;
	uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;

   init();
	// set encryption key
   if (WiFiDrv::wifiSetKey(ssid, strlen(ssid), key_idx, key, strlen(key)) != WL_FAILURE)
   {
	   do
	   {
		   delay(WL_DELAY_START_CONNECTION);
		   status = WiFiDrv::getConnectionStatus();
	   }while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
   }else{
	   status = WL_CONNECT_FAILED;
   }
   return status;
}

int WiFiClass::begin(char* ssid, const char *passphrase)
{
	uint8_t status = WL_IDLE_STATUS;
	uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;

   init();
    // set passphrase
    if (WiFiDrv::wifiSetPassphrase(ssid, strlen(ssid), passphrase, strlen(passphrase))!= WL_FAILURE)
    {
		   status = WiFiDrv::getConnectionStatus();
#if 0
 	   do
 	   {
 		   delay(WL_DELAY_START_CONNECTION);
 		   status = WiFiDrv::getConnectionStatus();
 	   }
	   while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
#endif
    }else{
    	status = WL_CONNECT_FAILED;
    }
    return status;
}
int WiFiClass::smart_config(unsigned int seconds)
{
	 uint8_t status = WL_IDLE_STATUS;
	 uint8_t attempts = WL_MAX_ATTEMPT_CONNECTION;
	
	init();
	if (WiFiDrv::wifiSetNetwork(seconds) != WL_FAILURE)
	{
		do
		{
			delay(WL_DELAY_START_CONNECTION);
			status = WiFiDrv::getConnectionStatus();
		}
		while ((( status == WL_IDLE_STATUS)||(status == WL_SCAN_COMPLETED))&&(--attempts>0));
	}else
	{
		status = WL_CONNECT_FAILED;
	}
	return status;
}

void WiFiClass::config(IPAddress local_ip)
{
	WiFiDrv::config(1, (uint32_t)local_ip, 0, 0);
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server)
{
	WiFiDrv::config(1, (uint32_t)local_ip, 0, 0);
	WiFiDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
	WiFiDrv::config(2, (uint32_t)local_ip, (uint32_t)gateway, 0);
	WiFiDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiClass::config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
	WiFiDrv::config(3, (uint32_t)local_ip, (uint32_t)gateway, (uint32_t)subnet);
	WiFiDrv::setDNS(1, (uint32_t)dns_server, 0);
}

void WiFiClass::setDNS(IPAddress dns_server1)
{
	WiFiDrv::setDNS(1, (uint32_t)dns_server1, 0);
}

void WiFiClass::setDNS(IPAddress dns_server1, IPAddress dns_server2)
{
	WiFiDrv::setDNS(2, (uint32_t)dns_server1, (uint32_t)dns_server2);
}

int WiFiClass::disconnect()
{
    return WiFiDrv::disconnect();
}

uint8_t* WiFiClass::macAddress(uint8_t* mac)
{
	init();
	uint8_t* _mac = WiFiDrv::getMacAddress();
	memcpy(mac, _mac, WL_MAC_ADDR_LENGTH);
    return mac;
}
   
IPAddress WiFiClass::localIP()
{
	IPAddress ret;
	WiFiDrv::getIpAddress(ret);
	return ret;
}

IPAddress WiFiClass::subnetMask()
{
	IPAddress ret;
	WiFiDrv::getSubnetMask(ret);
	return ret;
}

IPAddress WiFiClass::gatewayIP()
{
	IPAddress ret;
	WiFiDrv::getGatewayIP(ret);
	return ret;
}

char* WiFiClass::SSID()
{
    return WiFiDrv::getCurrentSSID();
}

uint8_t* WiFiClass::BSSID(uint8_t* bssid)
{
	uint8_t* _bssid = WiFiDrv::getCurrentBSSID();
	memcpy(bssid, _bssid, WL_MAC_ADDR_LENGTH);
    return bssid;
}

int32_t WiFiClass::RSSI()
{
    return WiFiDrv::getCurrentRSSI();
}

uint8_t WiFiClass::encryptionType()
{
    return WiFiDrv::getCurrentEncryptionType();
}


int8_t WiFiClass::scanNetworks()
{
	uint8_t attempts = 10;
	uint8_t numOfNetworks = 0;

	if (WiFiDrv::startScanNetworks() == WL_FAILURE)
		return WL_FAILURE;
 	do
 	{
 		delay(2000);
 		numOfNetworks = WiFiDrv::getScanNetworks();
 	}
	while (( numOfNetworks == 0)&&(--attempts>0));
	return numOfNetworks;
}

char* WiFiClass::SSID(uint8_t networkItem)
{
	return WiFiDrv::getSSIDNetworks(networkItem);
}

int32_t WiFiClass::RSSI(uint8_t networkItem)
{
	return WiFiDrv::getRSSINetworks(networkItem);
}

uint8_t WiFiClass::encryptionType(uint8_t networkItem)
{
    return WiFiDrv::getEncTypeNetworks(networkItem);
}

uint8_t WiFiClass::status()
{
    init();
    return WiFiDrv::getConnectionStatus();
}

int WiFiClass::hostByName(const char* aHostname, IPAddress& aResult)
{
	return WiFiDrv::getHostByName(aHostname, aResult);
}

WiFiClass WiFi;
