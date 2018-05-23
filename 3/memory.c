 * //		filename
 * /* */	original text
 * /* *///	note by myself

// scull.h

/*
 * The bare device is a variable-length region of memory.
 * Use a linked list of indirect blocks.
 *
 * "scull_dev->data" points to an array of pointers, each
 * pointer refers to a memory area of SCULL_QUANTUM bytes.
 *
 * The array (quantum-set) is SCULL_QSET long.
 */
#ifndef SCULL_QUANTUM
#define SCULL_QUANTUM 4000
#endif

#ifndef SCULL_QSET
#define SCULL_QSET 1000
#endif

/*
 * Representation of scull quantum sets.
 */
struct scull_qset {
	void **data;
	struct scull_qset *next;
};

struct scull_dev {
	struct scull_qset **data;	/* Pointer to first quantum set */
	int quantum;			/* the current quantum size */
	int qset;			/* the current array size */
	unsigned long size;		/* amount of data stored here */
	unsigned int access_key;	/* used by sculluid and scullpriv */
	struct semaphore sem;		/* mutual exclusion semaphore */
	struct cdev cdev;		/* Char device structure */
};

/*
 * The different configurable parameters
 */
extern int scull_quantum;
extern int scull_qset;

/*
 * Prototypes for shared functions
 */

int scull_trim(struct scull_dev *dev);

// main.c

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/slab.h>		/* void *kmalloc(size_t size, int flags)
				 * void kfree(void *ptr)
				 *///
#include <linux/types.h>	/* size_t */

#include "scull.h"		/* local definitions */

/*
 * Our parameters which can be set at load time.
 */

int scull_quantum = SCULL_QUANTUM;	/* number of bare scull devices */
int scull_qset =    SCULL_QSET;

module_param(scull_quantum, int, S_IRUGO);
module_param(scull_qset, int, S_IRUGO);

MODULE_LICENSE("Dual BSD/GPL");

/*
 * Empty out the scull device; must be called with the devicde
 * semapthore held,
 */
int scull_trim(struct scull_dev *dev)
{
	struct scull_qset *next, *dptr;
	int qset = dev->qset;	/* "dev" is not-null */
	int i;

	for (dptr = dev->data; dptr; dptr = next) { /* all the list items */
		if (dptr->data) {
			for (i = 0; i < qset; i++) {
				kfree(dptr->data[i]);
			}
			kfree(dptr->data);
			dptr->data = NULL;
		}
		next = dptr->next;
		kfree(dptr);
	}
	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;
	return 0;
}

