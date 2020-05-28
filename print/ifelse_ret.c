#ifdef _DEBUG
#define log printf
#else
#define log
#endif

int if_else_test(int val1, int val2, int val3) {
    int res = 0;
    if(val1 != 1) {
        log("Invalid val1\n");
        return;
    }
    val1 += 2;
    if(val2 != 1) {
        log("Invalid val2\n");
        return;
    }
    val2 += val1;
    if(val3 != 1) {
        log("Invalid val3\n");
        return;
    }
    val3 += val2;
    res = val3;
    printf("val is: %d", val3);

    return res;
}
