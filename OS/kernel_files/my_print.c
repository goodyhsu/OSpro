#include <linux/linkage.h>
#include <linux/kernel.h>

typedef long long int ll;

asmlinkage int sys_my_print(int pid, ll start_1, ll start_2, ll end_1, ll end_2){
	printk(KERN_INFO "[Project1] %d %lld.%09lld %lld.%09lld\n", pid, start_1, start_2, end_1, end_2);
	return 0;
}