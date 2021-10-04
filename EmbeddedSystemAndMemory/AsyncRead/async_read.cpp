#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/select.h>

using std::string;
using std::vector;

typedef int status_t;
#define OK 0
#define NO_ERROR OK

// Previous signal handler state, restored after first hit.
static struct sigaction gOrigSigactionINT;
static struct sigaction gOrigSigactionHUP;

void showTitleMsg()
{
    printf("=====================================\r\n");
    printf("||        demoMusicPlayerApp       ||\r\n");
    printf("=====================================\r\n");
    printf("Enter any command below\r\n");
    printf("t: for selecting a track to play\r\n");
    printf("l: for selecting a playlist\r\n");
    printf("p: start playback\r\n");
    printf("s: stop playback\r\n");
    printf("u: pause playback\r\n");
    printf("r: resume playback\r\n");
    printf("k: seek playback\r\n");
    printf("x: exit the application\r\n");
    printf("h: show this message again\r\n");
}

static void print(const char* format, ...)
{
    va_list arg, arg_copy;

    va_start(arg, format);
    vfprintf(stdout, format, arg);

    if (isatty(fileno(stdout)) != 1) {
        va_copy(arg_copy, arg);
        vfprintf(stderr, format, arg_copy);
        va_end(arg);
    }

    va_end(arg);
}

/*
 * Catch keyboard interrupt signals.  On receipt, the "stop requested"
 * flag is raised, and the original handler is restored (so that, if
 * we get stuck finishing, a second Ctrl-C will kill the process).
 */
static void signalCatcher(int signum)
{
    switch (signum) {
    case SIGINT:
    case SIGHUP:
        sigaction(SIGINT, &gOrigSigactionINT, NULL);
        sigaction(SIGHUP, &gOrigSigactionHUP, NULL);
        break;
    default:
        abort();
        break;
    }
}

/*
 * Configures signal handlers.  The previous handlers are saved.
 *
 * If the command is run from an interactive adb shell, we get SIGINT
 * when Ctrl-C is hit.  If we're run from the host, the local adb process
 * gets the signal, and we get a SIGHUP when the terminal disconnects.
 */
static status_t configureSignals()
{
    status_t err = NO_ERROR;
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = signalCatcher;
    if (sigaction(SIGINT, &act, &gOrigSigactionINT) != 0) {
        err = -errno;
        fprintf(stderr, "Unable to configure SIGINT handler: %s\r\n",
                strerror(errno));
        return err;
    }
    if (sigaction(SIGHUP, &act, &gOrigSigactionHUP) != 0) {
        err = -errno;
        fprintf(stderr, "Unable to configure SIGHUP handler: %s\r\n",
                strerror(errno));
        return err;
    }
    return err;
}

static void __exit(int value)
{
    print("\r\n<Return %d>\r\n", value);
    exit(value);
}

static struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode() {
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch() {
    int r = 0;
    unsigned char c = '\0';
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

bool readURLFromUser(string& url) {
    printf("Enter An URL to play: ");
    char buffer[500];
    scanf("%s", buffer);
    url = buffer;
    printf("[%s]\r\n", buffer);
    return true;
}

bool readPlayListFromUser(vector<string> playList) {
    char   buffer[500] = "\0";
    size_t count = 0;
    printf("Enter List of URLs to play [Press X ou ENTER key to end]:\r\n");
    do {
        buffer[0] = getch();
        if (buffer[0] != 'X' && buffer[0] != '\r' && buffer[0] != '\n' && buffer[0] != '\0') {
            scanf("%s", &buffer[1]);
            if (strncmp(buffer, "http://", 7) == 0 || strncmp(buffer, "https://", 8) == 0 ||
                strncmp(buffer, "rtsp://", 7) == 0 || strncmp(buffer, "file://", 7) == 0) {
                count++;
                playList.push_back(buffer);
                printf("%zu: [%s]\r\n", count, buffer);
            } else {
                printf("Invalid URL [%s] Ignoring\r\n", buffer);
            }
            buffer[0] = '\0';
        } else {
            printf("%zu entries in the playlist\r\n", count);
            break;
        }
    } while (true);

    return true;
}

int32_t readSeekPosition(int32_t range) {
    int32_t position = 0;
    printf("Enter Seek Position in milli seconds (range 0 - %d): ", range);
    scanf("%d", &position);
    return position;
}

class MusicPlayer {
public:
    MusicPlayer(const vector<string>& playList) : mCommand('t'), mPlayList(playList) {
        if (!mPlayList.empty()) {
            mURL = mPlayList[0];
        }
    }

    ~MusicPlayer() {
        cleanup();
    }

    status_t init() {
        status_t status = 0;
        return status;
    }

    void start() {
        bool bRunLoop = true;

        while (bRunLoop) {
            if (mCommand != '\0') {
                bRunLoop = processCommand();
                mCommand  = '\0';
            }
            if (kbhit()) {
                mCommand = getch();
            }
        }
    }

private:
    bool processCommand() {
        bool    bStatus = false;
        bool    bRunLoop = true;
        int32_t position = 0;

        switch (mCommand) {
        case 't':
            if (mURL.empty()) {
                readURLFromUser(mURL);
            }
            printf("Start\r\n");
            mURL.clear();
            break;
        case 'l':
            printf("Read Play List\r\n");
            readPlayListFromUser(mPlayList);
            break;
        case 'p':
            printf("Play\r\n");
            break;
        case 's':
            printf("Stop\r\n");
            break;
        case 'u':
            printf("Pause\r\n");
            break;
        case 'r':
            printf("Resume\r\n");
            break;
        case 'k':
            position = readSeekPosition(mDuration * 1000);
            printf("\r\nSeek to %d\r\n", position);
            break;
        case 'h':
            showTitleMsg();
            break;
        case 'x':
            printf("Exit the Player\r\n");
            bRunLoop = false;
            break;
        default:
            if (mSecondInvalidCmd) {
                printf("Exit the Player. Two invalid commands are entered\r\n");
                bRunLoop = false;
            } else {
                mSecondInvalidCmd = true;
            }
            break;
        }
        return bRunLoop;
    }

    void cleanup() {
    }

private:
    int            mDuration = 0;
    char           mCommand = 't';
    bool           mFirstTime = true;
    bool           mSecondInvalidCmd = false;
    string         mURL;
    vector<string> mPlayList;
};

int main(int argc, char** argv)
{
    set_conio_terminal_mode();
    showTitleMsg();

    // Configure signal handler.
    status_t err = configureSignals();
    if (err != NO_ERROR) {
        __exit(err);
    }

    vector<string> playList;
    for (int index = 1; index < argc; index++) {
        playList.push_back(argv[index]);
    }

    MusicPlayer player(playList);
    if (player.init() == OK) {
        player.start();
    }

    return 0;
}
