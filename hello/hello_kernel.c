#include <linux/module.h>
#include <linux/init.h>

int __init my_init(void)
{
	printk("hello_kernel - Hello Kernel!\n");
	return 0;
}

void __exit my_exit(void)
{
	printk("hello_kernel - Goodbye Kernel!\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("A simple hello world LKM");

module_init(my_init);
module_exit(my_exit);
