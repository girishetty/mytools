#include <stdio.h>

enum Errors {
    NoError = 0,
    Error1,
    Error2,
    Error3,
    Error4,
    Error5,
    Error6,
    Error7,
    Error8,
    Error9,
    Error10,
    Error11,
    Error12,
};

int printError(int error) {
    switch (error) {
    case NoError:
        printf("No Error\n");;
        return 0;
    case Error1:
        return printf("Error1\n");;
    case Error2:
        return printf("Error2\n");;
    case Error3:
        return printf("Error3\n");;
    case Error4:
        return printf("Error4\n");;
    case Error5:
        return printf("Error5\n");;
    case Error6:
        return printf("Error6\n");;
    case Error7:
        return printf("Error7\n");;
    default:
        break;
    }
    switch (error) {
    case Error8:
        return printf("Error8\n");;
    case Error9:
        return printf("Error9\n");;
    case Error10:
        return printf("Error10\n");;
    case Error11:
        return printf("Error11\n");;
    case Error12:
        return printf("Error12\n");;
    default:
        printf("Missing Error\n");;
        return 0;
    }
    return 0;
}

int main() {
    int error = 0;
    do {
        scanf("%d", &error);
        printError(error);
    } while (error != 0);

    return 0;
}

