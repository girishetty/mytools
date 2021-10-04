#include "stdafx.h"
#include <libusb.h>
#include <libusbhp.h>
#include "UsbMonitor.h"
#include "FlashToolDialog.h"

#ifdef TEST
#include <fstream>
using namespace std;
#endif

const INT gVID = 0x1286;
const INT gPID = 0x8174;
const INT USB_STABILITY_WAIT_TIME = 8000;  //in milli seconds
const INT USB_DISCONNECT_WAIT_TIME = 2000;  //in milli seconds

inline void UsbTopology::Set(UCHAR aBus, UCHAR aPort)
{
	sprintf(path, "%d-%d", aBus, aPort);
	iLength = 3;
}

inline void UsbTopology::Append(UCHAR aPort)
{
	sprintf(path + iLength, ".%d", aPort);
	iLength += 2;
}

UsbMonitor* UsbMonitor::CreateUsbMonitor(CFlashToolDialog* pHandler)
{
	UsbMonitor* pUsbMonitor = new UsbMonitor(pHandler);

	if (pUsbMonitor) {
		if (0 != pUsbMonitor->Init()) {
			delete pUsbMonitor;
			pUsbMonitor = NULL;
		}
	}

	return pUsbMonitor;
}

void UsbMonitor::EndUsbMonitor()
{
	delete this;
}

UsbMonitor::UsbMonitor(CFlashToolDialog* pHandler) : iHandle(NULL), iDeviceHandle(NULL), iHandler(pHandler)
{

}

UsbMonitor::~UsbMonitor()
{
	Exit();
}

int UsbMonitor::Init()
{
	int retVal = libusbhp_init(&iHandle);
	if (retVal == 0) {
		libusbhp_register_hotplug_listeners(iHandle, Attached, Detached, this);
		retVal = libusb_init(NULL);
	}
	return retVal;
}

void UsbMonitor::Exit()
{
	if (iHandle) {
		libusb_exit(NULL);
		libusbhp_exit(iHandle);
		iHandle = NULL;
	}
}

void UsbMonitor::Attached(struct libusbhp_device_t* pDevice, void* pUserData)
{
	UsbMonitor* pSelf = (UsbMonitor*)pUserData;
	if (pSelf && pDevice->idVendor == gVID && pDevice->idProduct == gPID) {
		pSelf->HandleUsbAttached();
	} else {
		vector<UsbTopology> usbTopology = pSelf->GetUsbTopology();
		pSelf->CheckTopologyChanged(usbTopology);
	}
}

void UsbMonitor::Detached(struct libusbhp_device_t* pDevice, void* pUserData)
{
	UsbMonitor* pSelf = (UsbMonitor*)pUserData;
	if (pSelf) {
		pSelf->HandleUsbDetached();
	}
}

void UsbMonitor::HandleUsbAttached()
{
	//wait for few seconds for usb device to be stable before accessing it
	Sleep(USB_STABILITY_WAIT_TIME);
	//A new USB device is added, update the Device List now
	iHandler->TriggerADB(TRUE);
}

void UsbMonitor::HandleUsbDetached()
{
	//One of the USB device is removed, so update the Device List again
	Sleep(USB_DISCONNECT_WAIT_TIME);
	iHandler->TriggerADB(FALSE);
}

vector<UsbTopology> UsbMonitor::GetUsbTopology()
{
	libusb_context* pCTX = NULL;
	struct libusb_device** deviceList;
	struct libusb_device* device;
	int deviceCount = 0;
	uint8_t bus;
	uint8_t portPath[8] = "";
	int pathCount = 0;
	vector<UsbTopology> topologyList;
	UsbTopology topology;

	if (libusb_get_device_list(pCTX, &deviceList) >= 0) {
		while ((device = deviceList[deviceCount++]) != NULL) {

			pathCount = libusb_get_port_numbers(device, portPath, sizeof(portPath));
			if (pathCount > 0) {
				bus = libusb_get_bus_number(device);
				topology.Set(bus, portPath[0]);
				for (int index = 1; index < pathCount; index++){
					topology.Append(portPath[index]); 
				}
				topology.End();
				topologyList.push_back(topology);
			}
		}
	}

	return topologyList;
}

void UsbMonitor::CheckTopologyChanged(vector<UsbTopology>& usbTopology)
{
	int index = 0;
	int found = -1;
	string previousTopology = iHandler->GetUsbTopology();
	string path;

	if (previousTopology.size() > 0) {
		//If we have a valid topology already, compare that with the current one.
		int count = usbTopology.size();
		for (; index < count; index++) {
			path = usbTopology.at(index).path;
			found = previousTopology.find(path);
			if (found != -1) {
				//Topology didn't change
				break;
			}
		}
		if (found == -1) {
			iHandler->NotifyChangeUsbTopology();
		}
	}
}

void UsbMonitor::UpdateUsbTopology(const UsbTopology* pUsbTopology)
{

}
