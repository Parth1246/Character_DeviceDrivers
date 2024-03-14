#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#include "ioctl_test.h"

/* Meta infomration */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parth Varsani");
MODULE_DESCRIPTION("A simple example for ioctl in LKM.");

/**
 * @brief This function is called, when the device file is opened.
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_number -> open was called!\n");
	return 0;
}


/**
 * @brief This function is called, when the device file is closed.
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_number -> close was called!\n");
	return 0;
}

/* Global variable for reading and writing */
int32_t answer = 42;

static long int my_ioctl(struct file *file, unsigned cmd, unsigned long arg) {
	struct mystruct test;

	switch(cmd) {
		case WR_VALUE:
			if(copy_from_user(&answer, (int32_t *) arg, sizeof(answer))) 
				printk("ioctl_example - Error copying data from user! \n");
			else 
				printk("ioctl_example - update the answer to %d\n",answer);
			break;
		case RD_VALUE:
			if(copy_to_user((int32_t *) arg,&answer,  sizeof(answer))) 
				printk("ioctl_example - Error copying data from user! \n");
			else 
				printk("ioctl_example - The answer was copied");
			break;
		case GREETER:
			if(copy_from_user(&test, (struct mystruct *) arg,  sizeof(test)))
				printk("ioctl_example - Error copying data from user! \n");
			else 
				printk("ioctl_example - %d greets to %s\n", test.repeat, test.name);
			break;
	}
	return 0;
}

//define struct which contains files operations
static struct file_operations fops = {

	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.unlocked_ioctl = my_ioctl
};

#define MYMAJOR 95

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {

	int retval;
	printk("Hello, Kernel!\n");
	/* register device number */
	retval = register_chrdev(MYMAJOR, "my_ioctl_example", &fops);
	if(retval == 0) {
		printk("ioctl_example - registered Device number Major: %d, Minor: %d\n", MYMAJOR,0);
	}
	if(retval > 0) {
		printk("ioctl_example - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
	}
	else if(retval < 0) {
		printk("Could not register device number!\n");
		return -1;
	}
	return 0;
}

/**
 * @brief This fucntion is called, When the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	unregister_chrdev(MYMAJOR, "my_ioctl_example");
	printk("Goodbye, Kenrel\n");	
}

module_init(ModuleInit);
module_exit(ModuleExit);
