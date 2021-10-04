#ifndef __USBHUB_PORT_TABLE__
#define __USBHUB_PORT_TABLE__

#include <string>
#include <vector>
using namespace std;

enum UsbHubType {
	D_LINK = 1,
	PLUGABLE = 2,
};

struct PortInfo {
	string iPortName;
	int iPortNum;

	PortInfo(const string& name, const int num) : iPortName(name), iPortNum(num) {}
};

/*
 * Abstract Class for UsbHubPortTable.
 * This class has a mechanism to map the USB Port Name (Topology) with the USB Port Slot in the USB HUB
 *
 * For every USB HUB that we are intending to use, there should be a corresponding derived UsbHubPortTable class
 * that defines/over-rides InitTable() function
 */
class UsbHubPortTable {
public:
	static UsbHubPortTable* CreateUsbHubPortTable(UsbHubType aType);
	virtual ~UsbHubPortTable() {}
	int GetPortNum(string aTopology) const;

protected:
	UsbHubPortTable() {}
	UsbHubPortTable(const UsbHubPortTable&);
	UsbHubPortTable& operator=(const UsbHubPortTable&);

	void AddEntry(const string& name, const int num) {
		iPortMappingTable.push_back(PortInfo(name, num));
	}

	/*
	 * Pure virtual function, that makes this class Abstract.
	 * For every USB HUB that we are intending to use, there should be a corresponding derived UsbHubPortTable class
	 * that defines/over-rides this function, where in they initialize the table
	 * 
	 * Update the Mapping Table with the USB Port Topology
	 * For example If the first port from left has the name/topology as 4.1, then add (name, num) pair as ("4.1", 1)
	 * While adding the Port information, begin with smaller topology (port name) first to have a
	 * consistent single GetPortNum implementation in the Baseclass
	 */
	virtual void InitTable() = 0;

protected:
	vector<PortInfo> iPortMappingTable;
};

// A MACRO that defines the class skeleton for every USB HUB that we are going to use
// This declares a class with the name as "UsbHubNameUsbHubPortTable"
#define DECLARE_USB_HUB_CLASS(UsbHubName)    \
class UsbHubName##UsbHubPortTable : public UsbHubPortTable { \
public: \
	virtual ~UsbHubName##UsbHubPortTable() {} \
protected: \
	UsbHubName##UsbHubPortTable() {} \
	UsbHubName##UsbHubPortTable(const UsbHubName##UsbHubPortTable &); \
	UsbHubName##UsbHubPortTable& operator=(const UsbHubName##UsbHubPortTable &); \
	/* Override InitTable function  */ \
	virtual void InitTable(); \
	friend class UsbHubPortTable; \
};

//Use the MACRO to declare the class for every USB HUB that we are going to use
DECLARE_USB_HUB_CLASS(DLink)
DECLARE_USB_HUB_CLASS(PlugAble)

#endif //__USBHUB_PORT_TABLE__
