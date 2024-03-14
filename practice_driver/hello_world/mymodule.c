#include <linux/module.h>
#include <linux/init.h>

/* Meta infomration */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parth Varsani");
MODULE_DESCRIPTION("This is practice hello world driver");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");
	return 0;
}

/**
 * @brief This fucntion is called, When the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("Goodbye, Kenrel\n");	
}

module_init(ModuleInit);
module_exit(ModuleExit);
