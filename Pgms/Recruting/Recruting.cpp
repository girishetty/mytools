#include <iostream>
#include <mutex>
#include <list>
#include <thread>
#include <unistd.h>

using namespace std;

#undef UINT64_MAX
#define UINT64_MAX 100L

constexpr uint32_t SALARY_MAX = 40000;     //40'000;
constexpr suseconds_t MILLI_SECOND = 1000; //1'000;

// All the things one needs to know about a ML employee
struct Employee
{
    uint64_t m_EmployeeId; // we do not want to be limited to (2^32 - 1) ML employees
    uint32_t m_SalaryUSD; // we do want to cap salary, to (2^32 - 1)

    Employee(const uint64_t& employeeId = 0L, uint32_t salaryUSD = 0) : m_EmployeeId(employeeId), m_SalaryUSD(salaryUSD) {}
};


// Base class for employee list manipulation
class ListMutator
{
public:
    ListMutator(mutex& mutex, list<Employee>& employees) :
        m_Mutex(mutex),
        m_Employees(employees)
    {
    }
    
    virtual ~ListMutator() {}

    thread StartThread()
    {
        return std::thread([=] { MainLoop(); });
    }

protected:

    virtual void MainLoop() = 0;

    mutex& m_Mutex;
    list<Employee>& m_Employees;
};


// Recruits employees
class Recruiter : public ListMutator
{
public:
    Recruiter(mutex& mutex, list<Employee>& employees) : ListMutator(mutex, employees) {}

protected:    
    void MainLoop() override
    {
        for (uint64_t employee = 1; employee <= UINT64_MAX; ++employee)
        {
            Employee leaper(employee);
            
            double lfLoveCoeff = (double)(rand()) / RAND_MAX;
            leaper.m_SalaryUSD = SALARY_MAX * lfLoveCoeff;

            SealTheDeal(leaper);

            // This is the worst thing that could happen ==> do not hire
            if (leaper.m_SalaryUSD >= SALARY_MAX)
            {
                continue;
            }
            
            // Bugzilla #4372: ID=7 is reserved (consult with HR for more)
            if (leaper.m_EmployeeId == 7)
            {
                continue;
            }

            m_Mutex.lock();
            m_Employees.push_back(leaper);
            m_Mutex.unlock();

            // Throttle down while we improve our facilities
            usleep(MILLI_SECOND);
        }
    }
    
private:
    void SealTheDeal(Employee& leaper)
    {
        // I can't see the harm in this - I gave the company everything I had in me!
        // TODO revisit in a couple of months
        if (leaper.m_EmployeeId == 4)
        {
            leaper.m_SalaryUSD = SALARY_MAX; //1'000'000;
        }
    }
};


// Processes employee records
class Welcomer : public ListMutator
{
    bool  m_bLastEmployeeWelcomed;
    mutex m_bDebugMutex;
public:
    Welcomer(mutex& mutex, list<Employee>& employees) : ListMutator(mutex, employees),
                                                        m_bLastEmployeeWelcomed(false) {}

protected:
    void MainLoop() override
    {
        Employee leaper;
        bool     bFound = false;
        uint64_t uLastEmployeeID = 0L;
        thread::id threadID = this_thread::get_id();

        while (!m_bLastEmployeeWelcomed) {
            if (true == m_Employees.empty()) {
                m_bDebugMutex.lock();
                cout << threadID << ": Welcomer waiting for new recruits..." << endl;
                m_bDebugMutex.unlock();
                usleep(MILLI_SECOND);
                continue;
            }
            
            // Critical section
            m_Mutex.lock();
            if (!m_Employees.empty()) {
                leaper = m_Employees.front();
                m_Employees.pop_front();
                bFound = true;
            }
            m_Mutex.unlock();

            if (bFound) {
                // TODO Allocate car spot
                // TODO Provide entrance badge
                // TODO Teach the basics of Krav Maga

                bFound = false;
                uLastEmployeeID = leaper.m_EmployeeId;
                m_bDebugMutex.lock();
                cout << threadID << ": New leaper: '" << uLastEmployeeID
                     << "' is good to go with salary='" << leaper.m_SalaryUSD << "' USD" << endl;
                m_bDebugMutex.unlock();
                if (uLastEmployeeID >= UINT64_MAX) {
                    m_bDebugMutex.lock();
                    cout << "Done welcoming all new recruits..." << endl;
                    m_bDebugMutex.unlock();
                    m_Mutex.lock();
                    m_bLastEmployeeWelcomed = true;
                    m_Mutex.unlock();
                }
            }
        }
    }
};


int main()
{
    srand(time(NULL));

    mutex accessMutex;
    list<Employee> employeesDb;

    Recruiter producer(accessMutex, employeesDb);
    Welcomer consumer(accessMutex, employeesDb);

    thread hProducer = producer.StartThread();
    thread hConsumer1 = consumer.StartThread();
    thread hConsumer2 = consumer.StartThread();

    cout << "We are hiring..." << endl;

    // Synchronize threads
    hProducer.join();
    hConsumer1.join();
    hConsumer2.join();

    cout << "All done!" << endl;

    return 0;
}

