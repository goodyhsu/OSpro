#include <string.h>
#include "header.h"

int cmp(const void *a, const void *b){
	proc *pa = (proc *)a;
	proc *pb = (proc *)b;
	return (pa-> ready - pb-> ready);
}

int main(){
	char policy[16];
	int process_num;
	scanf("%s%d", policy, &process_num);

	proc *procs = (proc *)malloc(process_num*sizeof(proc));
	assert(procs != NULL);
	for(int i = 0 ; i < process_num ; i++){
		scanf("%s%d%d", procs[i].name, &procs[i].ready, &procs[i].execu);
		procs[i].pid = -1;
	}

	//sort by ready time
	qsort(procs, process_num, sizeof(proc), cmp);

	int Policy = -1;
	if(strcmp(policy, "FIFO") == 0)
		Policy = 0;
	else if(strcmp(policy, "RR") == 0)
		Policy = 1;
	else if(strcmp(policy, "SJF") == 0)
		Policy = 2;
	else if(strcmp(policy, "PSJF") == 0)
		Policy = 3;
	if(Policy == -1){
		fprintf(stderr, "wrong policy\n");
		exit(0);
	}
	scheduler(Policy, process_num, procs);
}