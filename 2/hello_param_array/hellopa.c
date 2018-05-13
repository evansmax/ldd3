#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
MODULE_LICENSE("Dual BSD/GPL");

static int param_array[4] = {0,0,0,0};
static int param_array_num;
module_param_array(param_array, int, &param_array_num, S_IRUGO);

static int hello_init(void)
{
	int i = 0;
	while (i < param_array_num){
		printk(KERN_ALERT "param_array[%d] : %d\n", i, param_array[i]);
		i += 1;
	}
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, world\n");
}

module_init(hello_init);
module_exit(hello_exit);
