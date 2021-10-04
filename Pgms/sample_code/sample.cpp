// inserting into a vector
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <inttypes.h>

using namespace std;

class MyClass {
public:
    MyClass (int v = 0) : val(v) {
        cout << "MyClass::MyClass(" << this << ":" << val << ")" << endl;
    }

    ~MyClass () {
        cout << "MyClass::~MyClass(" << this << ":" << val << ")" << endl;
    }

    void Display() const {
        cout << "MyClass::Display(" << this << ":" << val << ")" << endl;
    }

    int GetVal() const { return val; }
    void SetVal(int val) { this->val = val; }

private:
    int val = 0;
};

void ClassTest() {
    MyClass* p = new MyClass(1);

    if (p) {
        cout << "MyClass:object: " << p << endl;
        delete p;
    }
}

struct MyStruct {
    int val = 10;
};

class PrivateClass {
public:
    static PrivateClass* Create(int val) {
        PrivateClass* self = new PrivateClass(val);
        return self;
    }

    void Release() {
        delete this;
    }

    void Display() const {
        cout<<"( " << this << ":" << mVal << " )" << endl;
    }

protected:
    PrivateClass(int val) : mVal(val) {
        cout<<"PrivateClass::PrivateClass";
        Display();
    }

    ~PrivateClass() {
        cout<<"PrivateClass::~PrivateClass";
        Display();
    }

    PrivateClass(const PrivateClass& );
    PrivateClass& operator=(const PrivateClass& );

private:
    int mVal = 0;
};

struct PrivateClassDeleter{
    inline void operator()(PrivateClass* ptr) const {
        if (ptr) {
            ptr->Release();
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
unique_ptr<MyClass> getIntPtr(int val) {
    unique_ptr<MyClass> ptr(new MyClass(val));
    ptr->Display();
    return ptr;
}

void UniquePtrTestOne() {
    unique_ptr<MyClass> ptr = getIntPtr(10);
    ptr->Display();
}

void UniquePtrTestTwo() {
    unique_ptr<PrivateClass, PrivateClassDeleter> ptr;
    ptr.reset(PrivateClass::Create(10));
    ptr->Display();
}

void UniquePtrTest() {
    UniquePtrTestOne();
    UniquePtrTestTwo();
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

typedef uint8_t UUID[4];

class UUIDCompare {
public:
    bool operator()(const UUID* argOne, const UUID* argTwo) const {
        return memcmp(argOne, argTwo, sizeof(UUID));
    }
};

typedef map<const UUID*, const MyClass&, UUIDCompare> MyMap_;

#define GET_MAP(ptr) ((MyMap_*)(ptr))

class MyMap {
public:
    MyMap();
    ~MyMap();
    bool init();
    void copy(const MyMap* initData);
    void add(const UUID& uuid, const MyClass& initData);
    const MyClass* get(const UUID& schemeUuid);

private:
    void* mData;
};


MyMap::MyMap() : mData(NULL)
{
}

MyMap::~MyMap()
{
    if (mData) {
        delete GET_MAP(mData);
    }
}

bool MyMap::init()
{
    if (!mData) {
        mData = new MyMap_;
    } else {
        GET_MAP(mData)->clear();
    }

    return (mData != NULL);
}

void MyMap::copy(const MyMap* initData)
{
    if (mData) {
        delete GET_MAP(mData);
        mData = NULL;
    }
    if (initData) {
        mData = initData->mData;
    }
}

void MyMap::add(const UUID& uuid, const MyClass& initData)
{
    MyMap_* mapData = NULL;
    pair<MyMap_::iterator, bool> ret;

    if (mData) {
        mapData = GET_MAP(mData);
        ret = mapData->insert(pair<const UUID*, const MyClass&>(&uuid, initData));
        if (ret.second) {
            cout << "Successfully inserted: " << ret.first->second.GetVal() << endl;
        } else {
            cout << "Element already exists: " << ret.first->second.GetVal() << endl;
        }
        cout << "======================" << endl;
        MyMap_::iterator it;
        for (it = mapData->begin(); it != mapData->end(); it++) {
            it->second.Display();
        }
        cout << "======================" << endl;
    }
}

const MyClass* MyMap::get(const UUID& schemeUuid)
{
    const MyClass* initData = NULL;
    MyMap_* mapData = NULL;
    MyMap_::const_iterator it;

    if (mData) {
        mapData = GET_MAP(mData);
        it = mapData->begin();
        if (memcmp(it->first, schemeUuid, sizeof(UUID)) == 0) {
            initData = &(it->second);
        } else {
            it = mapData->find(&schemeUuid);
            if (it != mapData->end()) {
                initData = &(it->second);
            } else {
                cout << "Could not find the element: " << endl; //<< it->second.GetVal() << endl;
            }
        }
        cout << "======================" << endl;
        MyMap_::iterator it_;
        for (it_ = mapData->begin(); it_ != mapData->end(); it_++) {
            it_->second.Display();
        }
        cout << "======================" << endl;
    }

    return initData;
}

void MapTest() {
    MyMap* p = new MyMap();
    UUID uuid1 = {0x01, 0x02, 0x03, 0x04};
    UUID uuid2 = {0x11, 0x12, 0x13, 0x14};
    UUID uuid3 = {0x21, 0x22, 0x23, 0x24};
    MyClass a(10);
    MyClass b(11);
    MyClass c(12);
    const MyClass* f = NULL;

    if (p) {
        if (p->init() == true) {
            p->add(uuid1, a);
            p->add(uuid2, b);
            p->add(uuid3, c);

            f = p->get(uuid3);
            if (f) {
                f->Display();
            }
            f = p->get(uuid1);
            if (f) {
                f->Display();
            }
            f = p->get(uuid2);
            if (f) {
                f->Display();
            }
        }
        delete p;
    }
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void PrintVector(const vector<uint8_t>& myvector) {
    vector<uint8_t>::const_iterator it;
    cout << "myvector contains (" << myvector.size() << "): ";
    for (it = myvector.begin(); it < myvector.end(); it++) {
        printf("%02X  ", *it);
    }
    cout << endl;
}

void AddStringToVector(const vector<uint8_t>& src,
                       const string& mode,
                       vector<uint8_t>& dst) {
    size_t i = 0;
    size_t j = 0;
    size_t size = 0;

    dst = src;
    i = src.size();
    size = i + mode.size();
    dst.resize(size);
    for (j = 0; i < size; i++, j++) {
        dst[i] = (uint8_t)mode[j];
    }
}

void AppendVector(vector<uint8_t>& myvector, const uint8_t* pData, size_t size) {
    myvector.insert(myvector.end(), pData, pData + size);
}

const uint8_t gData0[] = {0x01, 0x02, 0x03, 0x04};
const uint8_t gData1[] = {0x11, 0x12, 0x13, 0x14, 0x15};
const uint8_t gData2[] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

void VectorTest() {
    vector<uint8_t> myvector(gData0, gData0 + sizeof(gData0));

    PrintVector(myvector);
    AppendVector(myvector, gData1, sizeof(gData1));
    PrintVector(myvector);
    AppendVector(myvector, gData2, sizeof(gData2));
    PrintVector(myvector);
    AddStringToVector(myvector, "Girish", myvector);
    PrintVector(myvector);
    const uint8_t* p = nullptr;
    vector<uint8_t> myvector_(p, p + 0);
    PrintVector(myvector_);
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

const char* emptryStr = "";

void makeString(const char* pStr) {
    if (pStr == NULL) {
      pStr = emptryStr;
    }
    string str(pStr);
    cout << "String: " << str << endl;
}

void StringSearch() {
    const char* path = "to-search.txt";
    size_t readCount = 1000;
    char line[1024];
    ifstream source(path, ios::in);
    if (source.is_open()) {
        source.read(line, readCount);
        string buffer = line;
        auto foundIndex = buffer.find("mediaPresentationDuration=\"");
        if (foundIndex != string::npos) {
            foundIndex += 27;
            auto endIndex = buffer.find("\"", foundIndex + 1);
            auto len = endIndex - foundIndex;
            // Skip PT (in the begining) and S (in the end)
            auto temp = buffer.substr(foundIndex + 2, len - 3);
            cout << "Time: " << temp << endl;
            auto time = stod(temp);
            cout << "Time: " << temp << endl;
        }
    } else {
        cout << "Failed to open file" << endl;
    }
    source.close();
}

void parse_http(const string& http) {
    auto index = http.find(":");
    cout << "HTTP Header: " << http;
    if (index == string::npos) {
       index = http.find("HTTP/");
       if (index != string::npos) {
           for (; http[index] != ' '; index++);
           int val = stol(http.c_str() + index);
           cout << ": HTTP Response Code: " << val << endl;
       }
    }
}

// trim from start (in place)
static inline void trimTrailing(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void trimLeading(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    trimTrailing(s);
    trimLeading(s);
}

void parse_http_headrs(string headerField) {
    auto index = headerField.find(":");
    if (index != string::npos) {
        string key = headerField.substr(0, index);
        string value = headerField.substr(index + 1);
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        transform(value.begin(), value.end(), value.begin(), ::tolower);
        trim(key);
        trim(value);
        cout << "Key(" << key << ") : Value(" << value << ")" << endl;
    } else {
        // Find HTTP Response code
        index = headerField.find("HTTP/");
        if (index != string::npos) {
            for (; headerField[index] != ' '; index++);
            auto responseCode = stoi(headerField.c_str() + index);
            cout << "ResponseCode: " << responseCode << endl;
        }
    }
}

//To maintain all the Cookies for this session
static const size_t kMaxCookiePerDomain = 5;
static const size_t kMaxCookieSize = 200;
static size_t       gSessionCookieCount = 0;
static std::string  gSessionCookie;

static void removeOlderstSessionCookies(size_t removeSize = 0) {
    size_t cookiesSize = 0;
    size_t origCount = gSessionCookieCount;
    do {
        // Remove the very first cookie
        auto index = gSessionCookie.find("; ");
        if (index != string::npos) {
            // include "; " as well
            index += 2;
        }
        gSessionCookieCount--;
        gSessionCookie.erase(0, index);
        cookiesSize = gSessionCookie.size();
    } while ((cookiesSize + removeSize) >= kMaxCookieSize);
    cout << __func__ << ": Removed Entries: " << origCount - gSessionCookieCount << endl;
}

static void processCookie(string cookie) {
    // Fetch only the first name-value pair (name=value; <name=value;>) from the cookie field
    size_t index = cookie.find("; ");
    if (index != string::npos) {
        cookie.resize(index);
    }

    // Find the name of the cookie
    index = cookie.find("=");
    if (index != string::npos) {
        if (!gSessionCookie.empty()) {
            gSessionCookie.append("; ");
        }
        auto name = cookie.substr(0, index);
        index = gSessionCookie.find(name);
        if (index != string::npos) {
            // If said cookie is already there, remove it so that we can refresh it
            auto endIndex = gSessionCookie.find("; ", index);
            if (index != string::npos) {
                // include "; " as well
                endIndex = endIndex - index + 2;
            }
            gSessionCookie.erase(index, endIndex);
            gSessionCookieCount--;
        }
        if (gSessionCookieCount == kMaxCookiePerDomain) {
            // As we have too many cookies, delete one of the oldest cookie
            removeOlderstSessionCookies(0);
        }
        auto cookieSize = cookie.size();
        if ((gSessionCookie.size() + cookieSize) >= kMaxCookieSize) {
            // As we exceeded max cookie size, delete few of the oldest cookies
            removeOlderstSessionCookies(cookieSize);
        }
        // Now add the cookie
        gSessionCookieCount++;
        gSessionCookie.append(cookie);
    }
    cout << "Cookie: " << gSessionCookieCount <<
            " : " << gSessionCookie.size() <<
            " : " << gSessionCookie << endl;
}

void parseCookie() {
    cout << __func__ << "========================" << endl;
    std::string cookie = "f5avrbbbbbbbbbbbbbbbb=PPMHKMIJCHCKMNFHAMJPIODMHJPLCGMMHCFCCMFMAFGFIGJOCEGIIMPPOAKAPLJBFBJDAFJNBEIJHLJKBLPAGAKPIHMFPADKGBHAKNAGILBJGFJBLKKHDEFJCCBCCFKE; f5avrbbbbbbbbbbbbbbbb=HLOJPFPFFLOKLGPBAONJPLELDICFANMCIBAGFCMADPCNPPDJICEMCHDMAEFHFCCBFCADHGDCBEJFPDBPDMBAJMCBIHBMALKBMGHINKBEDNOGEADJDCMGDNAHECLBNMFK; f5avrbbbbbbbbbbbbbbbb=CNBAIDPCIGALJELPJIGGPAFIOJNJCKMABFGMMNGGFNBLBFLIBIOGILOBIINCGJJIAOHDFIDGCEPHKIHPBGCAPPACIHPKCOMKNNJPHMDINJPDKLAIICDJPBOLMILLEEGB; f5avrbbbbbbbbbbbbbbbb=GJJEFDHOGCPKOJFKPELBPOIMLCBEAHDKKEINGODIJAHNMHDHGHHFEHCHAEMFGKHLOEIDPJIHDEOPNNJPEKFALGMBIHLGKDAEFNGOLCCGMELBONLGHCPHKKIGMGPFJONP; f5avrbbbbbbbbbbbbbbbb=NPMJPCCHICALHFAPOOMGCALHLIIGAJOIBBLHEJLKMIOOMFKJBBKIOBEGBEMOPIPEHOPDEPAOHELLIIDLHNMAGFMDFHIMABODNANPGCANABPMOLBPPMNLIMNCMGCODNCO; f5avrbbbbbbbbbbbbbbbb=LJNOKNPLJJAKLLJDACJCCDPBKJENFEKJGCNFBJCAILPHGPBIFFOLPIHAKGHEACILCNCDNFKIEEILJNPGLBDACOBLIHAJPJELCCKJMDHKBDJIOJFAMJKDHKEMLIANGPCI; f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb=HLOJPFPFFLOKLGPBAONJPLELDICFANMCIBAGFCMADPCNPPDJICEMCHDMAEFHFCCBFCADHGDCBEJFPDBPDMBAJMCBIHBMALKBMGHINKBEDNOGEADJDCMGDNAHECLBNMFK; f5avrbbbbbbbbbbbbbbbb=CNBAIDPCIGALJELPJIGGPAFIOJNJCKMABFGMMNGGFNBLBFLIBIOGILOBIINCGJJIAOHDFIDGCEPHKIHPBGCAPPACIHPKCOMKNNJPHMDINJPDKLAIICDJPBOLMILLEEGB; f5avrbbbbbbbbbbbbbbbb=GJJEFDHOGCPKOJFKPELBPOIMLCBEAHDKKEINGODIJAHNMHDHGHHFEHCHAEMFGKHLOEIDPJIHDEOPNNJPEKFALGMBIHLGKDAEFNGOLCCGMELBONLGHCPHKKIGMGPFJONP; f5avrbbbbbbbbbbbbbbbb=NPMJPCCHICALHFAPOOMGCALHLIIGAJOIBBLHEJLKMIOOMFKJBBKIOBEGBEMOPIPEHOPDEPAOHELLIIDLHNMAGFMDFHIMABODNANPGCANABPMOLBPPMNLIMNCMGCODNCO; f5avrbbbbbbbbbbbbbbbb=LJNOKNPLJJAKLLJDACJCCDPBKJENFEKJGCNFBJCAILPHGPBIFFOLPIHAKGHEACILCNCDNFKIEEILJNPGLBDACOBLIHAJPJELCCKJMDHKBDJIOJFAMJKDHKEMLIANGPCI; f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb=CNBAIDPCIGALJELPJIGGPAFIOJNJCKMABFGMMNGGFNBLBFLIBIOGILOBIINCGJJIAOHDFIDGCEPHKIHPBGCAPPACIHPKCOMKNNJPHMDINJPDKLAIICDJPBOLMILLEEGB; f5avrbbbbbbbbbbbbbbbb=GJJEFDHOGCPKOJFKPELBPOIMLCBEAHDKKEINGODIJAHNMHDHGHHFEHCHAEMFGKHLOEIDPJIHDEOPNNJPEKFALGMBIHLGKDAEFNGOLCCGMELBONLGHCPHKKIGMGPFJONP; f5avrbbbbbbbbbbbbbbbb=NPMJPCCHICALHFAPOOMGCALHLIIGAJOIBBLHEJLKMIOOMFKJBBKIOBEGBEMOPIPEHOPDEPAOHELLIIDLHNMAGFMDFHIMABODNANPGCANABPMOLBPPMNLIMNCMGCODNCO; f5avrbbbbbbbbbbbbbbbb=LJNOKNPLJJAKLLJDACJCCDPBKJENFEKJGCNFBJCAILPHGPBIFFOLPIHAKGHEACILCNCDNFKIEEILJNPGLBDACOBLIHAJPJELCCKJMDHKBDJIOJFAMJKDHKEMLIANGPCI; f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb=GJJEFDHOGCPKOJFKPELBPOIMLCBEAHDKKEINGODIJAHNMHDHGHHFEHCHAEMFGKHLOEIDPJIHDEOPNNJPEKFALGMBIHLGKDAEFNGOLCCGMELBONLGHCPHKKIGMGPFJONP; f5avrbbbbbbbbbbbbbbbb=NPMJPCCHICALHFAPOOMGCALHLIIGAJOIBBLHEJLKMIOOMFKJBBKIOBEGBEMOPIPEHOPDEPAOHELLIIDLHNMAGFMDFHIMABODNANPGCANABPMOLBPPMNLIMNCMGCODNCO; f5avrbbbbbbbbbbbbbbbb=LJNOKNPLJJAKLLJDACJCCDPBKJENFEKJGCNFBJCAILPHGPBIFFOLPIHAKGHEACILCNCDNFKIEEILJNPGLBDACOBLIHAJPJELCCKJMDHKBDJIOJFAMJKDHKEMLIANGPCI; f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb=LJNOKNPLJJAKLLJDACJCCDPBKJENFEKJGCNFBJCAILPHGPBIFFOLPIHAKGHEACILCNCDNFKIEEILJNPGLBDACOBLIHAJPJELCCKJMDHKBDJIOJFAMJKDHKEMLIANGPCI; f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "Girish=Nokia; Shetty=Sooral";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "Girish=MagicLeap; Shetty=Sooral";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb1=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb2=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb3=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "f5avrbbbbbbbbbbbbbbbb4=MANFIFPMCEGFJLFAJNEPHBPHLADFGENFBMBOOKGHJEEBGKADKAFLEFADEAJLFLAPLFGDNKIDHEBDHENKCDNAGHEOIHHENHEGHPADCGEFEMMKLCIGOJDHLMMFECMJIDKC";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "GirishShetty=Marvell; Shetty=Sooral";
    if (!cookie.empty()) {
        processCookie(cookie);
    }

    cookie = "GirishShetty=MagicLeap; Shetty=Sooral";
    if (!cookie.empty()) {
        processCookie(cookie);
    }
}

void httpTest() {
    parse_http("HTTP/1.1 302");
    parse_http("HTTP/1.0   202");
    parse_http("HTTP/1.1    200");
    parse_http("201");
    parse_http("");
    parse_http_headrs("HTTP/1.1 302 ");
    parse_http_headrs("Access-Control-Allow-Origin: *");
    parse_http_headrs("Expires: Thu, 01 Jan 1970 00:00:00 GMT, -1");
    parse_http_headrs("Pragma: no-cache");
    parse_http_headrs("Location: http://csm-e-cedtuswxaws102j8-cb9kx896pqv.tls1.yospace.com/csm/live/129868781/1.mpd;jsessionid=650BEFCC42CE39497B051024F1DC4831.csm-e-cedtuswxaws102j8-cb9kx896pqv.bln1.yospace.com?yo.up=http://dfwlive-sponsored.akamaized.net/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/&yo.ac=true&yo.ec=503&yo.d.ut=no&externalId=WESPNHD-1964.dfw.720.mobile.abre.das");
    parse_http_headrs("Location:    http://csm-e-cedtuswxaws102j8-cb9kx896pqv.tls1.yospace.com/csm/live/129868781/1.mpd;jsessionid=650BEFCC42CE39497B051024F1DC4831.csm-e-cedtuswxaws102j8-cb9kx896pqv.bln1.yospace.com?yo.up=http://dfwlive-sponsored.akamaized.net/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/&yo.ac=true&yo.ec=503&yo.d.ut=no&externalId=WESPNHD-1964.dfw.720.mobile.abre.das");
    parse_http_headrs("Content-Type: application/dash+xml;charset=UTF-8   ");
}

void printTest() {
    uint64_t val = 1527528233323;
    int val_1 = 0;
    int val_2 = 1;

    printf("\n%01" PRIu64 ": %02" PRIu64 ": %" PRIu64 "\n", val, val, val);
    printf("\n%01d: %02d: %d\n", val_1, val_1, val_1);
    printf("\n%01d: %02d: %d\n", val_2, val_2, val_2);
}

void display(const std::vector<string>& items) {
    for (auto item : items) {
        std::cout << "item: " << item << std::endl;
    }
}

void stringVectorTest(bool flag) {
    std::cout << "Calling stringVectorTest with: " << flag << std::endl;
    std::vector<std::string> items;
    items.push_back("mute");
    items.push_back("volume 0.5");
    items.push_back("play");
    items.push_back("pause optional-resume");
    items.push_back("pause");

    auto it = std::find(items.begin(), items.end(), "pause optional-resume");
    if (it != items.end()) {
        if (flag) {
            items.erase(it);
        } else {
            std::cout << "Found" << std::endl;
        }
    }

    display(items);
}

void StringTest() {
    std::string str = "GIRISH";
    char* p = new char [str.size()];
    char* q = new char [str.size()];
    char* r = new char [str.size()];
    memcpy(p, str.data(), str.size());
    memcpy(q, str.data(), str.size());
    memcpy(r, str.data(), str.size());
    cout << "String: " << str << ":" << str.size() << ":" << p << ":" << q << ":" << r << endl;
}

void URLParsing(const std::string& url) {
    auto found = url.find("?bandwidth=");
    if (found == std::string::npos) {
        found = url.find("&bandwidth=");
    }                                  
    if (found != std::string::npos) {  
        auto beginPos = found + 11;
        auto endPos = url.find("&", beginPos);
        auto size = endPos - beginPos;
        std::cout << "Position: " << beginPos << ":" << endPos << ": Size: " << size << std::endl;
        auto value = url.substr(beginPos, size); 
        std::cout << "Value: " << value << std::endl;
    }
}

void testSizeFun() {
    struct SampleStruct {
        uint16_t display_chars[2 * 20];
    };

    SampleStruct a;
    SampleStruct* b = &a;
    std::cout << "SampleStruct Size: (" << sizeof(a.display_chars) << ":" << sizeof(b->display_chars) << ")" << std::endl;
    a.display_chars[0] = L'\0';
    std::cout << "SampleStruct Size: (" << sizeof(a.display_chars) << ":" << sizeof(b->display_chars) << ")" << std::endl;
}

void StructSize() {
    typedef struct {
        const char*   pURL;
        const uint8_t md5Sum[20];
    } TestURLSet;

    
    TestURLSet ts {nullptr, "Hello"};
    cout << "TestURLSet[" << sizeof(ts) << ":" << sizeof(ts.pURL) << ":" << sizeof(ts.md5Sum) << "]" << endl;
    TestURLSet* pts = &ts;
    cout << "TestURLSet[" << sizeof(*pts) << ":" << sizeof(pts->pURL) << ":" << sizeof(pts->md5Sum) << "]" << endl;
}

class Sample {
public:
    Sample(int val) : mVal(val) {}
    ~Sample() {}

    void Display() const {
        std::cout << "Val: " << mVal << std::endl;
    }

private:
    int mVal = 0;
};

#if 1
void foo(const Sample* p) {
  delete p;
}
#else
void foo(Sample* p) {
  delete p;
}
#endif

#ifdef DELETE_TEST
void DeleteTest() {
    Sample sam(10);
    foo(&sam);

   const int x=10;
   const int * a = &x;
   int * const b = new int;
   delete a;   //illegal - deleting pointer to constant
   delete b;   //legal
}
#endif

void test_test_test(const std::string& instreamID) {
#ifdef WRONG_USAGE
    int32_t accessbilityChannel = (instreamID.size() - 8) > 0 ? 1 : 2;
#else
    int32_t accessbilityChannel = (instreamID.size() > 8) ? 1 : 2;
#endif
    std::cout << "instreamID(" << instreamID.size() << "): " << instreamID
              << " accessbilityChannel: " << accessbilityChannel << std::endl;
}

void test_test() {
    test_test_test("Hao Zheng"); // size is 9
    test_test_test("hazheng");   // size is 7
    test_test_test("zheng");     // size is 5
    test_test_test("ha zheng");  // size is 8
}

int main(int argc, char** argv) {
#ifdef DELETE_TEST
    DeleteTest();
#endif
    if (argc == 2) {
        if (strcmp(argv[1], "-a") == 0) {
            VectorTest();
            ClassTest();
            MapTest();
            UniquePtrTest();
            httpTest();
            parseCookie();
            StringSearch();
            printTest();
        } else if (strcmp(argv[1], "-v") == 0) {
            VectorTest();
        } else if (strcmp(argv[1], "-c") == 0) {
            ClassTest();
        } else if (strcmp(argv[1], "-m") == 0) {
            MapTest();
        } else if (strcmp(argv[1], "-u") == 0) {
            UniquePtrTest();
        } else if (strcmp(argv[1], "-h") == 0) {
            httpTest();
        } else if (strcmp(argv[1], "-k") == 0) {
            parseCookie();
        } else if (strcmp(argv[1], "-s") == 0) {
            StringSearch();
        } else if (strcmp(argv[1], "-p") == 0) {
            printTest();
        } else if (strcmp(argv[1], "-ms") == 0) {
            makeString(NULL);
            makeString("Girish");
        } else if (strcmp(argv[1], "-sv") == 0) {
            stringVectorTest(false);
            stringVectorTest(true);
        }
    } else {
        StringTest();
        URLParsing("www.google.com?name=Girish&bandwidth=high");
        URLParsing("www.google.com?bandwidth=high&name=Girish");
        testSizeFun();
        cout << "Nothing to Test!" << endl;
    }

    return 0;
}
