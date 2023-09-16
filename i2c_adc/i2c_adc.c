#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/delay.h>

static struct of_device_id i2c_adc_ids[] = {
	{
		.compatible = "brightlight,myadc",
	},
       	{},
};
MODULE_DEVICE_TABLE(of, i2c_adc_ids);

static struct i2c_device_id i2c_adc[] = {
	{"my_adc", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, i2c_adc);

static int i2c_adc_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("i2c_adc - Now I am in the probe function\n");
	i2c_smbus_write_byte(client, 0x2);
	mdelay(600);
	printk("i2c_adc - Current ADC value: 0x%x\n", i2c_smbus_read_byte(client));
	return 0;
}

static void i2c_adc_remove(struct i2c_client *client)
{
	printk("i2c_adc - Now I am in the remove function!\n");
}

static struct i2c_driver i2c_adc_driver = {
	.probe = i2c_adc_probe,
	.remove = i2c_adc_remove,
	.id_table = i2c_adc,
	.driver = {
		.name = "i2c_adc",
		.of_match_table = i2c_adc_ids,
	},
};

int __init my_init(void)
{
	printk("i2c_adc - Hello Kernel!\n");
	return i2c_add_driver(&i2c_adc_driver);
}

void __exit my_exit(void)
{
	printk("i2c_adc - Goodbye Kernel!\n");
	i2c_del_driver(&i2c_adc_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes Roith");
MODULE_DESCRIPTION("A simple hello world LKM");

module_init(my_init);
module_exit(my_exit);
