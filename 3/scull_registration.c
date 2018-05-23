 * //		filename
 * /* */	original text
 * /* *///	note by myself

// scull.h

struct scull_dev {
	struct scull_qset *data;	/* Pointer to first quantum set		*/
	int quantum;			/* the current quantum size		*/
	int qset;			/* the current array size		*/
	unsigned long size;		/* amount of data stored here		*/
	unsigned int access_key;	/* used by sculluid and scullpriv	*/
	struct semaphore sem;		/* mutual exclusion semaphore		*/
	struct cdev cdev;		/* Char device stucture			*/
};

// main.c

#include <linux/module.h>
#include <linux/init.h>

#include <linux/kernel.h>		/* printk() */
#include <linux/fs.h>			/* file_operations *///
#include <linux/cdev.h>			/* cdev *///

#include "scull.h"			/* local definitions */

MODULE_LICENSE("Dual BSD/GPL");

struct file_operations scull_fops = {
	.owner = 	THIS_MODULE,
	.llseek =	scull_llseek,
	.read =		scull_read,
	.write =	scull_write,
	.ioctl =	scull_ioctl,
	.open =		scull_open,
	.release =	scull_release,
};

/*
 * Set up the char_dev structure for this device.
 */
static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err) {
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
	}
}

