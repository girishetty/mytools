
static void print_hex(const char* pWhat, const uint8_t* pData, const size_t uSize)
{
    size_t index = uSize;
    size_t i = 0;
    char*  buffer = NULL;

    if (pData && uSize > 0) {
        buffer = new char[uSize * 3];
        for (index = 0; index < uSize; index++) {
            sprintf(buffer + i, "%02X", pData[index]);
            i += 2;
            if ((index & 0x1F) == 0x1F) {
                sprintf(buffer + i, "\n");
                i += 1;
            }
        }
    }
    ALOGI("%s(%zu): %s", pWhat, uSize, buffer);
    if (buffer) {
        delete buffer;
    }
}
