#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUF_SIZE 1024
#define MYKEY	25

int main()
{
	int shmid;
	char *shmptr;

	if( (shmid = shmget(MYKEY,BUF_SIZE,IPC_CREAT))== -1 )
	{
		printf("shmget error!\n");
		exit(1);
	}

	if( (shmptr = shmat(shmid,0,0)) == (void*)-1 )
	{
		printf("shmat error!\n");
		exit(1);
	}

	while(1)
	{
		printf("input:%s\n");
		scanf("%s",shmptr);
	}



	exit(0);
}
