#include <iostream>
#include <fstream>

using namespace std;

//Implement a sample Media Source
class FileSource {
public:
    FileSource(const std::string& path) : iSize(-1), iCurrentPosition(0),
                                                     iSource(path, ios::in | ios::binary) {
        //Set up the current offset/posiiton (0)
        iCurrentPosition = iSource.tellg();
        //Set up the file size
        iSize = getAvailableDataSize();
    }

    ~FileSource() {
        close();
    }

    ssize_t readAt(size_t position, size_t size, uint8_t* buffer) {
        ssize_t readCount = -1;
        if ((ssize_t)position == iSize) {
            //End of Stream/File/Source
            readCount = 0;
        } else if (position < iSize && (buffer)) {
            if (position != iCurrentPosition) {
                //Seek to position
                iCurrentPosition = position;
                iSource.seekg(iCurrentPosition, ios::beg);
            }
            readCount = iSize - position;
            if (size < readCount) {
                readCount = size;
            }
            iSource.read((char*)buffer, readCount);
            iCurrentPosition += readCount;
        }
        return readCount;
    }

    ssize_t getSize() const {
        return iSize;
    }

    void close() {
        iSource.close();
        iSize = -1;
        iCurrentPosition = 0;
    }

protected:
    inline size_t getAvailableDataSize() {
        ssize_t currentSize = -1;
        if (iSource.is_open()) {
            //Seek to end of file
            iSource.seekg(0, ios::end);
            //Get the file size
            streampos fileSize = iSource.tellg();
            //Compute the size from current offset/statge
            currentSize = fileSize - iCurrentPosition;
            //Seek back to the original state/offset
            iSource.seekg(iCurrentPosition, ios::beg);
        }

        return currentSize;
    }

private:
    ssize_t   iSize;
    streampos iCurrentPosition;
    ifstream  iSource;
};

const size_t kDefaultReadSize = 100;

void FileIOTest(const char* fileName) {
    FileSource fRead(fileName);
    ssize_t fSize = fRead.getSize();
    ssize_t readCount = 0;
    size_t  position = 0;
    char    buffer[kDefaultReadSize + 1];

    cout << "File Size: " << fSize << endl;
    while (fSize >= position) {
        readCount = fRead.readAt(position, kDefaultReadSize, (uint8_t*)buffer);
        if (readCount == 0) {
            cout << "End of Stream" << endl;
            break;
        } else if (readCount == -1) {
            cout << "File Not Opened" << endl;
            break;
        } else {
            buffer[readCount] = '\0';
            cout << buffer;
            position += readCount;
        }
    }
    cout << "Done Reading" << endl;
}

int main(int argc, char** argv)
{
    if (argc == 2) {
        FileIOTest(argv[1]);
    } else {
        cout << "Nothing to Test!" << endl;
    }

    return 0;
}
