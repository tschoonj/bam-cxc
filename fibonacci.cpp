#include <QSharedMemory>
#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>

QSharedMemory *shared_memory;

void signal_handler(int sig) {
        if (sig == SIGINT) {
                std::cout << "SIGINT caught" << std::endl;
        }
        else if (sig == SIGTERM){
                std::cout << "SIGTERM caught" << std::endl;
        }

	//ensure proper cleanup when process is killed with Ctrl-C
	delete shared_memory;

        _exit(0);
}
int main (int argc, char *argv[]) {
        struct sigaction action;
        action.sa_handler = signal_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;

        sigaction(SIGINT, &action, NULL);
        sigaction(SIGTERM, &action, NULL);

	int fibonacci[1000];
	int i;

	fibonacci[0] = 0;
	fibonacci[1] = 1;

	for (i = 2 ; i < 1000 ; i++) {
		fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
	}

	shared_memory = new QSharedMemory("fibonacci");
	if (!shared_memory->create(sizeof(int)*1000)) {
		std::cerr << "Could not create shared_memory fibonacci: "<< shared_memory->errorString().toStdString().c_str() << std::endl;
		return 1;
	}
	else {
		std::cout << "shared_memory created with native key: " << shared_memory->nativeKey().toStdString().c_str() << std::endl;	
	}

	if (!shared_memory->isAttached()) {
		std::cerr << "Not attached to shared_memory!" << std::endl;
		return 1;
	}
	shared_memory->lock();
	memcpy((int *) shared_memory->data(), fibonacci, sizeof(int)*1000);
	shared_memory->unlock();

	sleep(3600);

	delete shared_memory;

	return 0;
}
