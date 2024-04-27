#include <iostream>
#include <cstring>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>

#define MAX_TIMERS 5
#define MAX_MESSAGE_LENGTH 128

void sighandler(int);
void printManPage();

int main(int argc, char **argv) {
    int fd;
    int pFile, oflags;
    struct sigaction action, oa;

    pFile = open("/dev/laserkeyboard", O_RDWR); // this is our kernel module!
    if (pFile == -1) {
        std::cout << "Failed to open the device." << std::endl;
        return -1;
    }

    // Setup signal handler
    memset(&action, 0, sizeof(action));
    action.sa_handler = sighandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGIO, &action, NULL);
    fcntl(pFile, F_SETOWN, getpid());
    oflags = fcntl(pFile, F_GETFL);
    fcntl(pFile, F_SETFL, oflags | FASYNC);

    std::cout << "Start of the code" << std::endl;
    pause();
    std::cout << "Unpaused" << std::endl;

    close(pFile); // Correct usage to close a file descriptor
    return 0;
}

// SIGIO handler
void sighandler(int signo) {
    std::cout << "Awaken!" << std::endl;
}
