#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Checker.h"

int main(int argc, char *argv[]) {
	
	int divisor = atoi(argv[1]);
	
	int dividends[4];
	dividends[0] = atoi(argv[2]);
	dividends[1] = atoi(argv[3]);
	dividends[2] = atoi(argv[4]);
	dividends[3] = atoi(argv[5]);
	
	int childpid[4];
    void *shared_memory[4];
    for (int i = 0; i < 4; ++i) {
        int fd[2]; // 0=read, 1=write
        pipe(fd);
        
        // Create a memory segment to share.
        int shmid = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
        
        // Attach the shared memory to our address space.
        shared_memory[i] = shmat(shmid, NULL, 0);
        // Write the shared memory ID to the pipe.
        char s_shmid[100];
        sprintf(s_shmid, "%d", shmid);
        write(fd[1], s_shmid, 100);
        
        childpid[i] = fork();
        if (childpid[i] <= 0) { // Child
            char first[10], second[10], file_descriptor[10];
            sprintf(first, "%d", divisor);
            sprintf(second, "%d", dividends[i]);
            sprintf(file_descriptor, "%d", fd[0]);
            
            shmctl(shmid, IPC_RMID, shared_memory[i]);
            execlp("Checker", "./Checker", first, second, file_descriptor, (char *)NULL);
            perror("Failed to execlp");
            exit(1);
        }
        else { // Parent
            printf("Coordinator: Forked process with ID %d.\n", childpid[i]);
            printf("Coordinator: Wrote shm ID %d to pipe (%d bytes).\n", shmid, sizeof(shmid));
            close(fd[1]);
        }
        
    }
    for (int i = 0; i < 4; ++i) {
        printf("Coordinator: Waiting on child process ID %d...\n", childpid[i]);
        int status;
        while (wait(&status) > 0);
        int result = atoi(shared_memory[i]);
        // Also missing this.
        printf("Coordinator: result %s read from shared memory: ", shared_memory[i]);
        if (result == 1)
            printf("%d is divisible by %d.\n", dividends[i], divisor);
        else
            printf("%d is not divisible by %d.\n", dividends[i], divisor);
        shmdt(shared_memory[i]);
    }
    
	printf("Coordinator: Exiting.\n");
	
	return 0;
}
