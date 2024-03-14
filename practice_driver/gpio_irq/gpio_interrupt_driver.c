#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

/* Meta infomration */ 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Parth Varsani");
MODULE_DESCRIPTION("A simple Loadable kernel module for a gpio interrupt");

/** Variable contains pin number 0 interrupt controller to which GPIO 17 is mapped to */
unsigned int irq_number;

/**
 * @brief Interrupt service routine is called, when interrupt is triggered
 */
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
	printk("gpio_irq: Interrupt was triggered and ISR was called!\n");
	return (irq_handler_t) IRQ_HANDLED;
}

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("gpio_irq: Loading module....\n");

	/* Setup the GPIO  */	
	if(gpio_request(17, "8m-mini-gpio-4")) {
		printk("Error:\nCan not allocate GPIO 17\n");
		return -1;
	}

	/* Set Gpio direction */
	if(gpio_direction_input(17)) {
		printk("Error:\nCan not set GPIO 17 to input\n");
		gpio_free(17);
		return -1;
	}

	/* Setup the interrupt */
	irq_number = gpio_to_irq(17);

	if(request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0) {
	printk("Error:\nCan not request interrupt nr: &d\n",irq_number);
	gpio_free(17);
	return -1;
	}	
	
	printk("Done\n");
	printk("GPIO 17 is mapped to IRQ number: %d\n",irq_number);
	return 0;
}

/**
 * @brief This fucntion is called, When the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("gpio_irq: UNBLOCKING module...\n");	
        free_irq(irq_number, NULL);
	gpio_free(17);
}

module_init(ModuleInit);
module_exit(ModuleExit);
