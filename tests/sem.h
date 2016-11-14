#ifndef __SEM_H_SEM__
#define __SEM_H_SEM__


int creat_sem(void);
int set_semvalue(int semid);
int sem_p(int semid);
int sem_v(int semid);
int del_sem(int semid);

#endif
