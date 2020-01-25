#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

int is_divisible(int divisor, int dividend) {
	int id = getpid();
	if ( dividend % divisor == 0 ) {
		printf("Checker process [%d]: %d *IS* divisible by %d.\n", id, dividend, divisor);
		printf("Checker process [%d]: Returning 1.\n", id);
		return 1;
	}
	else {
		printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", id, dividend, divisor);
		printf("Checker process [%d]: Returning 0.\n", id);
		return 0;
	}
}

int main(int argc, char *argv[]) {
    int id = getpid();
	printf("Checker process [%d]: Starting.\n", id);
    
    int fdread = atoi(argv[3]);
    char s_shmid[100];
    read(fdread, s_shmid, 100);
    int shmid = atoi(s_shmid);
    printf("Checker process [%d]: Read %d bytes containing shm ID %d\n", id, sizeof(shmid), shmid);
    
	int div = is_divisible(atoi(argv[1]), atoi(argv[2]));
    
    void *shared_memory = shmat(shmid, NULL, 0);
    sprintf(shared_memory, "%d", div);
    printf("Checker process [%d]: Wrote result (%d) to shared memory.\n", id, div);
    
    close(fdread);
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, shared_memory);
    
	return 0;
}
