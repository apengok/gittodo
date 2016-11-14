#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sem.h"

#define FTOK_FILE "/etc/profile"
#define TEST_FILE "./sem_file.dat"
#define STR_LEN 32
#define SHM_SIZE	256

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

typedef struct _tag_shm
{
	char buf[SHM_SIZE];
	unsigned short num;
}shm_t;
int creat_sem(void)
{
	int semid = 0;
	key_t key;

	key = ftok(FTOK_FILE,11);
	if(key == -1)
	{
		printf("%s : key = -1!\n",__func__);
		return -1;
	}

	semid = semget(key,1,IPC_CREAT|0666);
	if(semid == -1)
	{
		printf("%s : semid = -1!\n",__func__);
		return -1;
	}

	return semid;
}

int set_semvalue(int semid)
{
	union semun sem_arg;
	sem_arg.val = 1;

	if(semctl(semid,0,SETVAL,sem_arg) == -1)
	{
		printf("%s : can't set value for sem!\n",__func__);
		return -1;
	}
	return 0;
}

int sem_p(int semid)
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = -1;
	sem_arg.sem_flg = SEM_UNDO;

	if(semop(semid,&sem_arg,1) == -1)
	{
		printf("%s : can't do the sem_p!\n",__func__);
		return -1;
	}
	return 0;
}

int sem_v(int semid)
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = 1;
	sem_arg.sem_flg = SEM_UNDO;

	if(semop(semid,&sem_arg,1) == -1)
	{
		printf("%s : can't do the sem_v!\n",__func__);
		return -1;
	}
	return 0;
}

int del_sem(int semid)
{
	if(semctl(semid,0,IPC_RMID) == -1)
	{
		printf("%s : can't rm the sem!\n",__func__);
		return -1;
	}
	return 0;
}
int main()
{
	int semid,shmid;
	char buf[STR_LEN] = {0};
	int i = 8;
	void *pshm_addr = NULL;
	shm_t *pshm = NULL;

	/* get semid */
	semid = creat_sem();
	if(semid == -1)
	{
		printf("%s : semid = %d!\n",__func__,semid);
		return -1;
	}

	/* init semid */
	if(set_semvalue(semid))
	{
		printf("%s : set_semvalue failed!\n",__func__);
		return -1;
	}

	/* get shmid */
	shmid = shmget(ftok(FTOK_FILE,111),sizeof(shm_t),IPC_CREAT|0666);
	if(shmid == -1)
	{
		printf("%s : shmid = %d!\n",__func__,shmid);
		return -1;
	}

	/* current process link shmid */
	pshm_addr = shmat(shmid,0,0);
	if(pshm_addr == (void*)-1)
	{
		printf("%s : pshm_addr == (void*)0!\n",__func__);
		return -1;
	}
	
	pshm = pshm_addr;
	printf("read process semid is %d,shmid is %d!\n",semid,shmid);

	sleep(4);
	for(;;)
	{
		/* gain p operation */
		if(sem_p(semid))
		{
			printf("%s : sem_p failed!\n",__func__);
			return -1;
		}

		printf("enter read process!\n");
		printf("pshm->num is %d!\n",pshm->num);
		printf("pshm->buf is %s,",pshm->buf);
		printf("leave read process!\n\n");

		/* release v operation */
		if(sem_v(semid))
		{
			printf("%s : sem_v failed!\n",__func__);
			return -1;
		}

		if(!strncmp(pshm->buf,"end",3))
			break;

		sleep(2);
	}

	/* delete semid */
	if(del_sem(semid))
	{
		printf("%s : del_sem failed!\n",__func__);
		return -1;
	}

	/* detache process and shm */
	if(shmdt(pshm_addr) == -1)
	{
		printf("%s : shmdt failed!\n",__func__);
		return -1;
	}

	/* delete shm */
	if(shmctl(shmid,IPC_RMID,0) == -1)
	{
		printf("%s : shmctl failed!\n",__func__);
		return -1;
	}

	printf("BYE!\n");
	return 0;
}
