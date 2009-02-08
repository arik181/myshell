#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

int main()
{
	struct timeval seedtime;
	gettimeofday(&seedtime, NULL);
	srand (seedtime.tv_usec);

	int randomten = rand() % 10;
	sleep(randomten);

	exit(0);
}
