 * //		filename
 * /* */	original text
 * /* *///	note by myself

// scull.h

struct scull_dev {
	struct scull_qset *data;	/* Pointer to first quantum set */
	int quantum;			/* the current quantrm size */
	int qset;			/* the current array size */
	unsigned long size;		/* amount of date stored here */
	unsigned int access_key;	/* used by sculluid and scullpriv */
	struct semaphore sem;		/* mutual exclusion semaphore */
	struct cdev cdev;		/* Char device structure */
}

// main.h

/*
 * Open and close
 */

#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>	/* container_of(pointer, container_type, container_filed) *///
#include <linux/fcntl.h>	/* O_ACCMODE */

#include "scull.h"		/* local definitions */

MODULE_LICENSE("Dual BSD/GPL");

int scull_open(struct inode *inode, struct file *flip)
{
	struct scull_dev *dev; /* device information */

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; /* for other methods */

	/* now trim to 0 the length of the device if open was write-only */
	if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {
		if (down_interruptible(&dev->sem))
		{
			return -ERESTATSYS;
		}
		scull_trim(dev); /* ignore errors */
		up(&dev->sem);
	}
	return 0;	/* success */
}

int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations scull_fops = {
	.owner =	THIS_MODULE,
	.llseek =	scull_llseek,
	.read =		scull_read,
	.write =	scull_write,
	.ioctl =	scull_ioctl,
	.open =		scull_open,
	.release =	scull_release,
};

