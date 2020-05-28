#include "Member.h"

void Performjob(Member& mem) {
	ILabAssistance* pLab = mem;
	const LabInfo* pLabInfo = pLab->LabDetails();
}

int main() {
	MemberData memData = {"Girish Shetty", 888};
	Member* pMem = new Member(memData);

	Performjob(*pMem);

	delete pMem;
	return 0;
}
