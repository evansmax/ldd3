 * //		filename
 * /* */	original text
 * /* *///	note by myself

// scull.h

#ifndef SCULL_MAJOR
#define SCULL_MAJOR 0		/* dynamic major by default */
#endif

#ifndef SCULL_NR_DEVS
#define SCULL_NR_DEVS 4		/* scull0 through scull3 */
#endif

extern int scull_major;		/* main.c */
extern int scull_nr_devs;


// main.c

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init>

#include <linux/kernel.h>	/* printk() */
#include <linux/types.h>	/* dev_t *///
#include <linux/fs.h>		/* filesystem
				 * int register_chrdev_region(dev_t first, unsigned int count, char *name);
			   	 * int alloc_chrdev_region(dev_t *dev, unsigned int firstminor,unsigned int count,char *name);
			   	 * void unregister_chrdev_region(dev_t first, unsigned int count);
				 *///
#include <linux/cdev.h>		/* kdev_t.h [
			   	 *	int MAJOR(dev_t dev);
				 *	int MINOR(dev_t dev);
				 *	dev_t MKDEV(unsigned int major, unsigned int minor);
				 *	]
			 	 *///

#include "scull.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */

int scull_major =   SCULL_MAJOR;
int scull_minor =   0;
int scull_nr_devs = SCULL_NR_DEVS;	/* number of bare scull devices */

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_nr_devs, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPL");

int scull_init_module(void)
{
	int result;
	dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_minor, "scull");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARING "scull: can't get major %d\n", scull_major);
		return result;
	}
}

module_init(scull_init_module);

