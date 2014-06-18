#ifndef __USB_MONITOR_H__
#define __USB_MONITOR_H__

#include <vector>
using namespace std;

struct libusbhp_device_t;
struct libusb_device_handle;
class CFlashToolDialog;

const int TOPOLOGY_MAX_LEN = 20;

struct UsbTopology {
	CHAR path[TOPOLOGY_MAX_LEN];

	inline UsbTopology() : iLength(0) {
		Reset();
	}

	inline void Reset() {
		path[0] = '\0';
	}

	inline void Set(unsigned char aBus, unsigned char aPort);
	inline void Append(unsigned char aPort);
	inline void End() {
		path[iLength] = '\0';
	}

private:
	int iLength;
};

class UsbMonitor {
public:
	static UsbMonitor* CreateUsbMonitor(CFlashToolDialog* pHandler);
	void EndUsbMonitor();

private:
	UsbMonitor(CFlashToolDialog* pHandler);
	~UsbMonitor();
	UsbMonitor(const UsbMonitor& );
	UsbMonitor& operator=(const UsbMonitor&);

	int Init();
	void Exit();

	inline CFlashToolDialog* Handler() const {
		return iHandler;
	}

	void HandleUsbAttached();
	void HandleUsbDetached();
	vector<UsbTopology> GetUsbTopology();
	void CheckTopologyChanged(vector<UsbTopology>& usbTopology);
	void UpdateUsbTopology(const UsbTopology* pUsbTopology);

	static void Attached(struct libusbhp_device_t* pDevice, void* pUserData);
	static void Detached(struct libusbhp_device_t* pDevice, void* pUserData);

private:
	struct libusbhp_t* iHandle;
	struct libusb_device_handle* iDeviceHandle;
	CFlashToolDialog* iHandler;
	UsbTopology iUsbTopology;
};

#endif
