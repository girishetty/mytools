# include <vector>
# include <memory>
# include <iostream>

union Date {
  struct {
    char day[2];
    char seperator1;
    char month[2];
    char seperator2;
    char year[2];
  };
  char date[11];
};

void DateUnion() {
  Date birthday_1 = { .date = "20/08/1980" };
  Date birthday_2 = { .date = "01-05-1980" };
  Date birthday_3 = { .date = {0x30, 0x31, 0x00, 0x30, 0x35, 0x00, 0x31, 0x39, 0x38, 0x30, 0x00 }};
  std::cout << "Birthday#1: " << birthday_1.day << ":" << birthday_1.month << ":" << birthday_1.year << std::endl;
  std::cout << "Birthday#2: " << birthday_2.day << ":" << birthday_2.month << ":" << birthday_2.year << std::endl;
  std::cout << "Birthday#3: " << birthday_3.day << ":" << birthday_3.month << ":" << birthday_3.year << std::endl;
}

// Enabling this will crash while accessing EventData::std::vector<int>
//#define DEFAULT_CONSTRUCTOR
// Enabling this will work fine, but NOTE that if we introduce one more data type, it may cause trouble.
//#define VECTOR_CONSTRUCTOR
// Enabling this will crash while accessing EventData::std::vector<int>
//#define SP_CONSTRUCTOR
// This will be the best approach, but that needs some extra work overloading all the constructors
// and creating the object first, and then moving to union
#define OVERLOAD_ALL_CONSTRUCTOR

union EventData {
  unsigned long         mLongData;
  double                mDoubleData;
  std::vector<int>      mIntData;
  std::vector<char>     mCharData;
  std::unique_ptr<int>  mIntPtrData;
  std::shared_ptr<char> mCharPtrData;

  ~EventData() {}

#ifdef DEFAULT_CONSTRUCTOR
  // Not initializing any members of the union
  EventData() {}
#elif defined VECTOR_CONSTRUCTOR
  // Initializing std::vector<int> member of the union
  EventData() : mIntData(0) {}
#elif defined SP_CONSTRUCTOR
  // Initializing std::unique_ptr<int> of the union
  EventData() : mIntPtrData(nullptr) {}
#elif defined OVERLOAD_ALL_CONSTRUCTOR
  EventData(unsigned long longData) : mLongData(longData) {}
  EventData(double doubleData) : mDoubleData(doubleData) {}
  EventData(const std::vector<int>& intData) : mIntData(std::move(intData)) {}
  EventData(const std::vector<char>& charData) : mCharData(std::move(charData)) {}
  EventData(std::unique_ptr<int>& intPtrData) : mIntPtrData(std::move(intPtrData)) {}
  EventData(std::shared_ptr<char>& charPtrData) : mCharPtrData(std::move(charPtrData)) {}

  // disabling copy, move constructor and operator
  EventData() = delete;
  EventData(const EventData &) = delete;
  EventData(EventData&&) = delete;
  EventData& operator=(const EventData &) = delete;
  EventData& operator=(EventData&&) = delete;
#endif
};

enum EventType {
  LongData = 0,
  DoubleData,
  IntData,
  CharData,
  IntPtrData,
  CharPtrData,
};

struct Event {
  EventType mEvent;
#ifdef OVERLOAD_ALL_CONSTRUCTOR
  std::unique_ptr<EventData> mEventData;
#else
  EventData mEventData;
#endif

  Event() {}

  inline const char* getEventName() const {
    switch (mEvent) {
    case LongData:
      return "LongData event";
    case DoubleData:
      return "DoubleData event";
    case IntData:
      return "IntData event";
    case CharData:
      return "CharData event";
    case IntPtrData:
      return "IntPtrData event";
    case CharPtrData:
      return "CharPtrData event";
    }
  }

};

#ifdef OVERLOAD_ALL_CONSTRUCTOR
void DisplayEvents(const Event& event) {
  switch (event.mEvent) {
  case LongData:
    std::cout << "LongData: " << event.mEventData->mLongData << std::endl;
    break;
  case DoubleData:
    std::cout << "DoubleData: " << event.mEventData->mDoubleData << std::endl;
    break;
  case IntData:
    std::cout << "IntData: " << event.mEventData->mIntData.size() << std::endl;
    break;
  case CharData:
    std::cout << "CharData: " << event.mEventData->mCharData.size() << std::endl;
    break;
  case IntPtrData:
    std::cout << "IntPtrData: " << event.mEventData->mIntPtrData.get() << std::endl;
    break;
  case CharPtrData:
    std::cout << "CharPtrData: " << event.mEventData->mCharPtrData.get() << std::endl;
    break;
  }
}

void CreateEventUnion(EventType type) {
  Event event;
  event.mEvent = type;

  std::cout << "Lets create " << event.getEventName() << std::endl;
  switch (type) {
  case LongData:
    event.mEventData = std::make_unique<EventData>(200lu);
    break;
  case DoubleData:
    event.mEventData = std::make_unique<EventData>(200.0);
    break;
  case IntData:
    {
      std::vector<int> temp = {1, 2, 3, 4, 5, 6, 7, 8};
      event.mEventData = std::make_unique<EventData>(temp);
    }
    break;
  case CharData:
    {
      std::vector<char> temp = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
      event.mEventData = std::make_unique<EventData>(temp);
    }
    break;
  case IntPtrData:
    {
      auto temp = std::make_unique<int>(10);
      event.mEventData = std::make_unique<EventData>(temp);
    }
    break;
  case CharPtrData:
    {
      auto temp = std::make_shared<char>('A');
      event.mEventData = std::make_unique<EventData>(temp);
    }
    break;
  }

  DisplayEvents(event);
  std::cout << "Sucessfully created " << event.getEventName() << std::endl;
}

#else

void DisplayEvents(const Event& event) {
  switch (event.mEvent) {
  case LongData:
    std::cout << "LongData: " << event.mEventData.mLongData << std::endl;
    break;
  case DoubleData:
    std::cout << "DoubleData: " << event.mEventData.mDoubleData << std::endl;
    break;
  case IntData:
    std::cout << "IntData: " << event.mEventData.mIntData.size() << std::endl;
    break;
  case CharData:
    std::cout << "CharData: " << event.mEventData.mCharData.size() << std::endl;
    break;
  case IntPtrData:
    std::cout << "IntPtrData: " << event.mEventData.mIntPtrData.get() << std::endl;
    break;
  case CharPtrData:
    std::cout << "CharPtrData: " << event.mEventData.mCharPtrData.get() << std::endl;
    break;
  }
}

void CreateEventUnion(EventType type) {
  Event event;
  event.mEvent = type;

  std::cout << "Lets create " << event.getEventName() << std::endl;
  switch (type) {
  case LongData:
    event.mEventData.mLongData = 200;
    break;
  case DoubleData:
    event.mEventData.mDoubleData = 200.0;
    break;
  case IntData:
    {
      std::vector<int> temp = {1, 2, 3, 4, 5, 6, 7, 8};
      event.mEventData.mIntData = temp;
    }
    break;
  case CharData:
    {
      std::vector<char> temp = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
      event.mEventData.mCharData = temp;
    }
    break;
  case IntPtrData:
    event.mEventData.mIntPtrData = std::make_unique<int>(10);
    break;
  case CharPtrData:
    event.mEventData.mCharPtrData = std::make_shared<char>('A');
    break;
  }

  DisplayEvents(event);
  std::cout << "Sucessfully created " << event.getEventName() << std::endl;
}
#endif

void EventUnion() {
  CreateEventUnion(LongData);
  CreateEventUnion(DoubleData);
  CreateEventUnion(IntData);
  CreateEventUnion(CharData);
  CreateEventUnion(IntPtrData);
  CreateEventUnion(CharPtrData);
}

int main() {
  //DateUnion();
  EventUnion();
  return 0;
}

