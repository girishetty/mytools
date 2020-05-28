#include "Member.h"

bool IsTimeInRange(UINT startTime, UINT endTime);

Member::Member(const MemberData& pMemData) :
	  m_pData (NULL)
	, m_pCourseInfo(NULL)
	, m_pLabInfo(NULL)
	, m_pLibraryInfo(NULL)
{
	m_pData = new MemberData;
	//deep copy all data from pMemData to m_pData
}

Member::~Member()
{
	delete m_pData;
	delete m_pCourseInfo;
	delete m_pLabInfo;
	delete m_pLibraryInfo;
}

const MemberData* Member::MemberInfo()
{
	return m_pData;
}

const CourseInfo* Member::Course()
{
	return m_pCourseInfo;
}

void Member::SetCourse(const CourseInfo* pCourse)
{
	delete m_pCourseInfo;
	m_pCourseInfo = new CourseInfo;
	//deep copy all data from pCourse to m_pCourseInfo
}

const LabInfo* Member::LabDetails()
{
	return m_pLabInfo;
}

void Member::SetLabDetails(const LabInfo* pInfo)
{
	delete m_pLabInfo;
	m_pLabInfo = new LabInfo;
	//deep copy all data from pInfo to m_pLabInfo
}

const LibInfo* Member::LibraryInfo()
{
	return m_pLibraryInfo;
}

void Member::SetLibraryInfo(const LibInfo* pInfo)
{
	delete m_pLibraryInfo;
	m_pLibraryInfo = new LibInfo;
	//deep copy all data from pInfo to m_pLibraryInfo
}

//Conversion functions
Member::operator ILabAssistance*() { 
	ILabAssistance* p = NULL;
	// Return the ILabAssistance* based on precondition or state of the object
	// Get system time and if the time is in range 13:00 to 16:00
	UINT startTime = 13;
	UINT endTime = 16;
	bool isTimeInRange = IsTimeInRange(startTime, endTime);
	if(isTimeInRange)
	{
		p = (ILabAssistance*)this;
	}
	return p;
}

Member::operator ILibraryIncharge*() {
	ILibraryIncharge* p = NULL;
	// Return the ILibraryIncharge* based on precondition or state of the object
	// Get system time and if the time is in range 13:00 to 16:00
	UINT startTime = 17;
	UINT endTime = 20;
	bool isTimeInRange = IsTimeInRange(startTime, endTime);
	if(isTimeInRange)
	{
		p = (ILibraryIncharge*)this;
	}
	//return the ILabAssistance* based on precondition or state of the object
	return p;
}

bool IsTimeInRange(UINT startTime, UINT endTime)
{
	//Get System time and check for the validation
	bool isInRange = false;
	UINT currentTime = 0; //GetSystemTime();
	if(currentTime >= startTime && currentTime <= endTime)
	{
		isInRange = true;
	}

	return isInRange;
}
