
void dumpLog(const char* what, const uint8_t* array, ssize_t bytes) {
    String8 result;
    for (int i = 0; i < bytes; i++) {
        result.appendFormat("%02x", array[i]);
    }

    ALOGI("GIRISH: [%s: %zu: %zu] [%s]", what, bytes, result.size(), result.string());
}
