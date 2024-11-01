#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <signal.h>
#include <cjson/cJSON.h>
#include "server.h"
#include "hardware.h"
#include <sys/stat.h>

int main(int argc, char **argv){
// printf hello world
	// printf("Hello World!\n");
	server_start();
	return 0;
}