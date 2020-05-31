#ifndef MEMBER_H
#define MEMBER_H

typedef unsigned int UINT;
#define INTERFACE struct
#define NULL 0

struct CourseInfo {
    char* m_pName;
    UINT  m_uID;
	UINT  m_uHours;
};

struct LabInfo {
    char* m_pName;
    UINT  m_uID;
	UINT  m_uHours;
};

struct LibInfo {
    char* m_pName;
    UINT  m_uID;
	UINT  m_uHours;
};

struct MemberData {
    char* m_pName;
    UINT  m_uID;
};

INTERFACE IStudent {
	virtual const CourseInfo* Course() = 0;
	virtual void SetCourse(const CourseInfo* pCourse) = 0;
};

INTERFACE ILabAssistance {
	virtual const LabInfo* LabDetails() = 0;
	virtual void SetLabDetails(const LabInfo* pInfo) = 0;
};

INTERFACE ILibraryIncharge {
	virtual const LibInfo* LibraryInfo() = 0;
	virtual void SetLibraryInfo(const LibInfo* pInfo) = 0;
};

INTERFACE ILecture {
};

class Member : public IStudent
             , private ILabAssistance
			 , private ILibraryIncharge
//			 , private ILecture
{
public:
	Member(const MemberData& pMemData);
	~Member();

	const MemberData* MemberInfo();
	//INTERFACE IStudent
	virtual const CourseInfo* Course();
	virtual void SetCourse(const CourseInfo* pCourse);

   //INTERFACE ILabAssistance
	virtual const LabInfo* LabDetails();
	virtual void SetLabDetails(const LabInfo* pInfo);

	//INTERFACE ILibraryIncharge
	virtual const LibInfo* LibraryInfo();
	virtual void SetLibraryInfo(const LibInfo* pInfo);

	//Conversion functions
	operator ILabAssistance*();
	operator ILibraryIncharge*();

private:
	MemberData*  m_pData;
	CourseInfo*  m_pCourseInfo;
	LabInfo*     m_pLabInfo;
	LibInfo*     m_pLibraryInfo;
};

#endif /* MEMBER_H */
