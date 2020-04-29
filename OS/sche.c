#include "header.h"
#include <sys/wait.h>

void time_unit(){ volatile unsigned long i; for(i=0;i<1000000UL;i++); } 

int pol_FIFO(proc *procs, int finish){
	if(procs[finish].pid == -1) //not ready
		return -1;
	return finish;
}

int q[5000000] = {};
int q_head = 0, q_tail = 0;

int pol_RR(proc *procs, int running, ll *RR_time){//if(*RR_time > 500)fprintf(stderr, "wrong RR %lld\n", *RR_time);
	if(q_head == q_tail){
		*RR_time = 0;
		return -1;
	}
	if(running == -1 && q_head == 0 && procs[q[q_head]].execu > 0)
		return q[q_head];
	if(running == -1){
		*RR_time = 0;
		return q[++q_head];
	}

	if(*RR_time == tq){
		int id = q[q_head++];
		q[q_tail++] = id;
		*RR_time = 0;//fprintf(stderr, "RR %lld\n", *RR_time);
	}
	return q[q_head];
}


int pol_SJF(proc *procs, int process_num, int running){
	if(running != -1)
		return running;
	int ret = -1;
	int min = INF;
	for(int i = 0 ; i < process_num ; i++){
		if(procs[i].pid != -1 && procs[i].execu > 0 && procs[i].execu < min){
			min = procs[i].execu;
			ret = i;
		}
	}
	return ret;
}

int pol_PSJF(proc *procs, int process_num){
	int ret = -1;
	int min = INF;
	for(int i = 0 ; i < process_num ; i++){
		if(procs[i].pid != -1 && procs[i].execu > 0 && procs[i].execu < min){
			min = procs[i].execu;
			ret = i;
		}
	}
	return ret;
}

void scheduler(int policy, int process_num, proc *procs){
	//initializing
	ll time = 0; //current time
	int finish = 0; //number of processes that have finished their tasks
	int running = -1; //process that is being running 
	int arrive_index = 0; //number of processes that have arrived
	int RR_index = -1;
	ll RR_time = 0;

	for(int i = 0 ; i < process_num ; i++)
		procs[i].pid = -1;

	//assign CPU
	assign_CPU(getpid(), 0);

	//scheduling
	while(1){
		#ifdef DE
		if(running != -1)
			fprintf(stderr, "running = %d exec = %d\n", running, procs[running].execu);
		#endif

		if(finish == process_num)
			break;
		if(running != -1 && procs[running].execu > 0){
			procs[running].execu--;
			if(policy == 1)//RR
				RR_time++;
		}
		if(running != -1 && procs[running].execu == 0){
			finish++;
			//wait
			if(waitpid(procs[running].pid, NULL, 0) < 0)
				fprintf(stderr, "failed to wait\n");
			running = -1;
			if(finish == process_num)
				break;
		}

		//check if there is any newly arriving process
		while(arrive_index < process_num && procs[arrive_index].ready <= time){
			if(policy == 1){
				q[q_tail++] = arrive_index;
			}
			pid_t new_p = create(arrive_index, procs);
			assert(new_p > 0);
			procs[arrive_index].pid = new_p;
			if(stop(new_p) < 0)
				fprintf(stderr, "stop error\n");
			arrive_index++;//fprintf(stderr, "%d arrived at %lld\n", arrive_index, time);
		}

		//determine the next process to run
		int next;
		switch(policy){
			case 0:
				next = pol_FIFO(procs, finish);
				break;
			case 1:
				next = pol_RR(procs, running, &RR_time);
				break;
			case 2:
				next = pol_SJF(procs, process_num, running);
				break;
			case 3:
				next = pol_PSJF(procs, process_num);
				break;
			default:
				fprintf(stderr, "wrong Policy\n");
				break;
		}

		#ifdef DE
		if(next != -1)
			fprintf(stderr, "next = %d exec = %d finish = %d\n", next, procs[next].execu, finish);
		#endif

		//context switch
		if(next != running){
			if(running != -1){
				if(stop(procs[running].pid) < 0)
					fprintf(stderr, "stop error\n");
			}
			if(next != -1){
				if(resume(procs[next].pid) < 0)
					fprintf(stderr, "resume error\n");
			}

			#ifdef DEE
			fprintf(stderr, "%d switch to %d\n", running, next);
			#endif
			
		}

		time_unit(), time++;
		running = next;
	}
}