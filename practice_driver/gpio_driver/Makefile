obj-m := my_gpio_driver.o
KERN_DIR =/lib/modules/6.2.0-39-generic/build

all:
	make -C ${KERN_DIR} M=${PWD} modules

clean:
	make -C ${KERN_DIR} M=${PWD} clean

help: 
	make -C ${KERN_DIR} M=${PWD} help
