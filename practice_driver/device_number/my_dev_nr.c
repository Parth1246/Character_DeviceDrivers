#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/* Meta infomration */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parth Varsani");
MODULE_DESCRIPTION("It registers the device number and implements some callback functions.");

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

//define struct which contains files operations

static struct file_operations fops = {

	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

#define MYMAJOR 95

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {

	int retval;
	printk("Hello, Kernel!\n");
	/* register device number */
	retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
	if(retval == 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d\n", MYMAJOR,0);
	}
	if(retval > 0) {
		printk("dev_nr - registered Device number Major: %d, Minor: %d\n", retval>>20, retval&0xfffff);
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
	unregister_chrdev(MYMAJOR, "my_dev_nr");
	printk("Goodbye, Kenrel\n");	
}

module_init(ModuleInit);
module_exit(ModuleExit);
