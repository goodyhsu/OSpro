#define _GNU_SOURCE
#include "header.h"
#include <sched.h>
#include <sys/syscall.h>


int stop(pid_t pid){
	struct sched_param par;
	
	par.sched_priority = 1;
	int ret = sched_setscheduler(pid, SCHED_FIFO, &par);
	
	if (ret < 0) {
		fprintf(stderr, "failed to stop\n");
		return -1;
	}
	return ret;
}

int resume(pid_t pid){
	struct sched_param par;
	
	par.sched_priority = 99;
	int ret = sched_setscheduler(pid, SCHED_FIFO, &par);
	
	if (ret < 0) {
		fprintf(stderr, "failed to resume\n");
		return -1;
	}
	return ret;
}

pid_t create(int index, proc *procs){
	pid_t ret = fork();

	if(ret == 0){ //child
		assign_CPU(getpid(), 1);
		ll start_1, start_2, end_1, end_2;

		//block 
		if(stop(getpid()) < 0)
			fprintf(stderr, "failed to self-block\n");

		//get start time
		if(syscall(TIME, &start_1, &start_2) < 0)
			fprintf(stderr, "failed to get start time\n");

		//run
		for(int i = 0 ; i < procs[index].execu ; i++)
			time_unit();

		//get end time
		if(syscall(TIME, &end_1, &end_2) < 0)
			fprintf(stderr, "failed to get end time\n");

		//print to dmesg
		if(syscall(PRINT, getpid(), start_1, start_2, end_1, end_2) < 0)
			fprintf(stderr, "dmesg error\n");

		fprintf(stdout, "%s %d\n", procs[index].name, getpid());
		exit(0);
	}

	//scheduler
	return ret;
}

int assign_CPU(pid_t pid, int CPU_id){
	cpu_set_t C;
	CPU_ZERO(&C);
	CPU_SET(CPU_id, &C);

	if(sched_setaffinity(pid, sizeof(C), &C) < 0)
		fprintf(stderr, "failed to set affinity\n");
}