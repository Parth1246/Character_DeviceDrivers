//include below header files for any character driver

#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/slab.h>

/* 3 device number */
dev_t device_number;

/* 12 class var */
static struct class *dev_class;

//16 structure function declaration
static int skull_open(struct inode *inode, struct file *file);
static int skull_release(struct inode *inode, struct file *file);
ssize_t skull_read(struct file *flip, char __user *buf, size_t len, loff_t *off);
ssize_t skull_write(struct file *flip, const char __user *buf, size_t len, loff_t *off);


/* 5 File Operation Structure
 * which is nothing but API pointers */
static struct file_operations skull_fops = {
	.owner = THIS_MODULE,
	.read = skull_read,
	.write = skull_write,
	.open = skull_open,
	.release =  skull_release
};

/* kernel memory act as device */
#define MEM_SIZE 1024
uint8_t *kernel_buffer;

/* 6 cdev structure 
 * structure of char device driver */
struct cdev skull_cdev;

/* 1 Driver init function */
static int __init char_init(void)
{
	/* 2 Get the dynamic Device number(combination of major & minor number).
	 * this alloc_chrdev_region function allocates a range of char device numbers.
	 * major number will be chosen dynamically and returned in dev
	 * along with the first minor number.*/
	if(alloc_chrdev_region(&device_number,0,1,"skull")<0){
		pr_info("alloc_chrdev_region() failed\n");
		return -1;
	}

	/* 4 Initialization of cdev */
	cdev_init(&skull_cdev,&skull_fops);

	/* 7 add owner macro to owner field of cdev
	 * to add owner and file trucutre details */
	skull_cdev.owner = THIS_MODULE;
	skull_cdev.ops = &skull_fops;

	/* 8 Now register the charecter device driver using cdev_add func
	 * it will add the device represented by pointer to the system,
	 * making it live eventually. */
	if( cdev_add(&skull_cdev, device_number, 1) < 0)
	{
		pr_info("cdev_add() failed\n");
		goto r_class;
	}

	/* 10 if we successfully registered the char device 
	 * now its time to register the class */
	if ( (dev_class = class_create(THIS_MODULE,"skull_class")) == NULL)
	{
		pr_info("class class_create() failed\n");
		goto r_class;
	}

	/* 11 after registering the class now we can create the physical char device driver 
	 * in short device file , for whihc device_create func is used. */
	if ( (device_create(dev_class, NULL, device_number, NULL, "skull_device")) == NULL){
		pr_info("class class_create() failed\n");
		goto r_device;

	}

	return 0;

/* 13 whatever the class is created successfully
 * first  we need to destry that then we need to unregister 
 * the region and then we get return = -1 that means our init func is failed. 
 *
 * but if it got succeeded then it will go to return 0; and not to these r_device and r_class */
r_device:
	class_destroy(dev_class);

/* 9 it will unregister out region we will get from the macro(given by alloc_chrdev_region func
 * we need to unregister it. */
r_class:
	unregister_chrdev_region(device_number, 1);	
	return -1;
}

static void __exit char_cleanup(void)
{
	// 14 sequence of exit the char dirver.
	/* 1  Device distory */
	device_destroy(dev_class, device_number);

	/* 2 class Distroy */
	class_destroy(dev_class);

	/* unregister cdev */
	cdev_del(&skull_cdev);
	/* Deallocation of chardriver region for free memory */
	unregister_chrdev_region(device_number, 1);

}

//17 defination of those function is given below.

static int skull_open(struct inode *inode, struct file *file)
{
        if( (kernel_buffer = kmalloc(MEM_SIZE,GFP_KERNEL)) == 0){
                pr_info("kmalloc() failed can not allocate memory\n");
                return -1;
        }
        pr_info("kmalloc() allocate memory device open\n");
        return 0;
}

static int skull_release(struct inode *inode, struct file *file)
{
        kfree(kernel_buffer);
        pr_info("kmalloc() dellocate memory device close\n");
        return 0;
}

ssize_t skull_read(struct file *flip, char __user *buf, size_t len, loff_t *off)
{
        copy_to_user(buf, kernel_buffer, MEM_SIZE);
        pr_info("copy_to_user() device read()\n");
        return MEM_SIZE;
}

ssize_t skull_write(struct file *flip,const char __user *buf, size_t len, loff_t *off)
{
        copy_from_user(kernel_buffer, buf, len);
        pr_info("copy_from_user() device write()\n");
        return len;
}


//15 now add some module description

module_init(char_init);
module_exit(char_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("parth varsani");
MODULE_DESCRIPTION("Charecter Device Driver dummy");
MODULE_INFO(board,"host x86 machine");
