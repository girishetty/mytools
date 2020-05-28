#include "stdafx.h"
#include "UsbHubPortTable.h"


UsbHubPortTable* UsbHubPortTable::CreateUsbHubPortTable(UsbHubType aType)
{
	UsbHubPortTable* pUsbHubPortTable = NULL;
	switch (aType) {
	case D_LINK:
		pUsbHubPortTable = new DLinkUsbHubPortTable();
		break;
	case PLUGABLE:
		pUsbHubPortTable = new PlugAbleUsbHubPortTable();
		break;
	}

	if (pUsbHubPortTable) {
		pUsbHubPortTable->InitTable();
	}

	return pUsbHubPortTable;
}

int UsbHubPortTable::GetPortNum(string aTopology) const
{
	//Compare the incoming topology with the Port Mapping Table
	//If the topology ends, with one of the mapping table entry, we know the port number
	int index = iPortMappingTable.size() - 1;
	int topologySize = aTopology.size();
	int entrySize = -1;
	int subStrSize = -1;
	int portNum = -1;

	//Start from the end, as we need to beat the ambiguity
	for (; index >= 0; index--) {
		entrySize = iPortMappingTable.at(index).iPortName.size();
		subStrSize = topologySize - entrySize;
		if (subStrSize > 0) {
			if (iPortMappingTable.at(index).iPortName == aTopology.substr(subStrSize)) {
				portNum = iPortMappingTable.at(index).iPortNum;
				break;
			}
		}
	}
	return portNum;
}

//Override InitTable for DLink
void DLinkUsbHubPortTable::InitTable() {
	//D-Link USB HUB Model No: DUB-H7 has below configuraiton from left to right (on front side)
	AddEntry(".4", 1);             //Port-1
	AddEntry(".3", 2);             //Port-2
	AddEntry(".2", 3);             //Port-3
	AddEntry(".5.5", 4);           //Port-4
	AddEntry(".5.2", 5);           //Port-5
	AddEntry(".5.3", 6);           //Port-6
	AddEntry(".5.4", 7);           //Port-7
}

//Override InitTable for PlugAble
void PlugAbleUsbHubPortTable::InitTable() {
	//PLUGABLE USB HUB Model No: USB3-HUB7-81X has below configuraiton from left to right (on front side)
	AddEntry(".2", 5);             //Port-5
	AddEntry(".3", 6);             //Port-6
	AddEntry(".4", 7);             //Port-7
	AddEntry(".1.1", 1);           //Port-1
	AddEntry(".1.2", 2);           //Port-2
	AddEntry(".1.3", 3);           //Port-3
	AddEntry(".1.4", 4);           //Port-4
}
