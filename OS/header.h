#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#define tq 500 //time quantum for RR
#define INF 2147483647
#define TIME 333
#define PRINT 334

typedef long long int ll;

typedef struct proc{
	char name[32];
	int ready; //ready time
	int execu; //execution time
	pid_t pid; //-1 if has not been created
}proc;

void time_unit();

void scheduler(int policy, int process_num, proc *procs);

int stop(pid_t pid);

int resume(pid_t pid);

pid_t create(int index, proc *procs); //return new pid

int assign_CPU(pid_t pid, int CPU_id);