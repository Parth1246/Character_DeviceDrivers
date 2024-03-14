#include <linux/module.h> //we need to add this because we are using LKM - loadable kernel module

//why static because these functions are related to only this module only, other function or program cant call this

static int __init hello_init(void) 
{
	pr_info("hello world\n"); // for printing a logs in kernel use pr_info (kernel function) //latest way to print
	//printk(KENR_INFO"hello world\n"); //older way
	return 0;
}

static void __exit hello_cleanup(void)
{
	pr_info("goodbye world\n");
}

//we nedd to now register the function using kernal macros
//syntex -> module_init(which module)
module_init(hello_init);
module_exit(hello_cleanup);

//kenrel gives some macros to give some details to our module
MODULE_LICENSE("GPL"); //GPL -> general public licence
MODULE_AUTHOR("parth varsani");
MODULE_INFO(board,"linux_host x86");
MODULE_DESCRIPTION("this is just a hellow world demo");
