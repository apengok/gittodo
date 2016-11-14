#include <sys/sem.h>

#include "sem.h"

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

	if(semop(semid,sem_arg,1) == -1)
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
