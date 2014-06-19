#include <stdio.h>

const char* string1 = "3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe56808c213f17c81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d562e8e9582f5ef375f0a4ae20ed86e935de81230b59b73fb4302cd95d770c65b40aaa065f2a5e33a5a0bb5dcaba43722130f042f8ec85b7c2070";

const char* string2 = "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a19d84aba34d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512472e8e8f8db3f9635c1949e640c621854eba0d79eccf52ff111284b4cc61d11902aebc66f2b2e436434eacc0aba938220b084800c2ca4e693522643573b2c4ce35050b0cf774201f0fe52ac9f26d71b6cf61a711cc229f77ace7aa88a2f19983122b11be87a59c355d25f8e4";

const char* string3 = "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b01c909ba7696cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea125d298e8883f5e9305f4b44f915cb2bd05af51373fd9b4af511039fa2d96f83414aaaf261bda2e97b170fb5cce2a53e675c154c0d9681596934777e2275b381ce2e40582afe67650b13e72287ff2270abcf73bb028932836fbdecfecee0a3b894473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4ce";


const char* string4 = "315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3";

const char* string5 = "271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027";


const char* string6 = "466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83";

const char* string7 = "32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac650e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef121336cb85ccb8f3315f4b52e301d16e9f52f904";

const char* question1 = "4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81";

const char* question2 = "5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253";

const char* question3 = "69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329";

const char* question4 = "770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451";

void print_out(const char* str, unsigned int len) {
    unsigned int index = 0;
    unsigned int count = 0;

    printf("output is:\nconst unsigned char ciphertext_1[] = {\n");

    for(; index < len ; index += 6) {
        printf("%.6s", str + index);
        if ((count & 0xF) == 0xF) {
            printf("\n");
        }
        count++;
    }

    printf("\n};\n");
}

void get_hex_string(const char* string) {

    unsigned int size = strlen(string);

    char* out_string = (char*) malloc(size * 5);
    unsigned int index1 = 0;
    unsigned int index2 = 0;

    if (out_string) {
        for(; index2 < size ; index2 += 2) {
            out_string[index1++] = '0';
            out_string[index1++] = 'x';
            out_string[index1++] = string[index2];
            out_string[index1++] = string[index2 + 1];
            out_string[index1++] = ',';
            out_string[index1++] = ' ';
        }
        out_string[index1 - 1] = '\0';
        printf("input is:\n%s\n", string);
        print_out(out_string, index1);
        free(out_string);
    }
}

void print_hex(const unsigned char* str, const unsigned int len) {
    unsigned int index = 0;

    printf("ASCII: %s\nHEX:\n", str);

    for(; index < len ; index ++) {
        printf("%.2x", str[index]);
        if ((index & 0xF) == 0xF) {
            printf("\n");
        }
    }

    printf("\n");
}

const char* hex_string = "546865204d6167696320576f72647320";
const unsigned char hex_string_[] = {
0x54, 0x68, 0x65, 0x20, 0x4d, 0x61, 0x67, 0x69, 0x63, 0x20, 0x57, 0x6f, 0x72, 0x64, 0x73, 0x20,
};

int main() {
#if 0
    get_hex_string(string1);
    get_hex_string(string2);
    get_hex_string(string3);
    get_hex_string(string4);
    get_hex_string(string5);
    get_hex_string(string6);
    get_hex_string(string7);

    print_hex("The ", 4);
    print_hex(" probably ", 10);
    print_hex("ctionary", 8);
    print_hex("graphy", 6);
    print_hex("ncryption", 9);
    print_hex("ipher", 5);
    print_hex("rithm", 5);
    print_hex("The secret ", 11);
    print_hex("er use ", 7);
    print_hex("nment", 5);
    print_hex("ey more than once", 17);
    get_hex_string(question1);
    get_hex_string(question2);
    get_hex_string(question3);
    get_hex_string(question4);
#endif
    //get_hex_string(hex_string);
    print_hex(hex_string_, 16);
}

