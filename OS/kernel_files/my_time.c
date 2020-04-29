#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

typedef long long int ll;

asmlinkage int sys_my_time(ll *a, ll *b){
	struct timespec t;
    getnstimeofday(&t);

	*a = t.tv_sec;
	*b = t.tv_nsec;
	return 0;
}