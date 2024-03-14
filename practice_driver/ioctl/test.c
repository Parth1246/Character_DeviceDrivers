#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl_test.h"

int main() {
	int answer;
	struct mystruct test = {4, "Parth"};
	int dev = open("/dev/dummy", O_WRONLY);
	if(dev == -1) {
		printf("opening was not possible\n");
		return -1;
	}

	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is now %d\n", answer);
	
	answer = 123;

	ioctl(dev, WR_VALUE, &answer);
	ioctl(dev, RD_VALUE, &answer);
	printf("The answer is now %d\n", answer);

	ioctl(dev, GREETER, &test);

	printf("opening was successfull!\n");
	close(dev);
	return 0;
}
