#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);

static int hello_init(void)
{
	int i = 0;
	while (i < howmany){
		printk(KERN_ALERT "%d : Hello, %s\n", i, whom);
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
