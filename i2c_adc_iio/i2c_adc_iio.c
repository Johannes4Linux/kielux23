#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

struct i2c_adc_iio {
	struct i2c_client *client;
};

static int i2c_adc_read_raw(struct iio_dev * indio_dev, struct iio_chan_spec const * chan, int *val, int *val2, long mask) {
	struct i2c_adc_iio *adc = iio_priv(indio_dev);

	if(mask == IIO_CHAN_INFO_RAW) {
		*val = i2c_smbus_read_byte(adc->client);
	}
	else
		return -EINVAL;
	return IIO_VAL_INT;
}

static const struct iio_chan_spec my_adc_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	}
};

static const struct iio_info my_adc_info = {
	.read_raw = i2c_adc_read_raw,
};

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
	struct iio_dev *indio_dev;
	struct i2c_adc_iio *adc;

	printk("i2c_adc - Now I am in the probe function\n");

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(struct iio_dev));
	if(!indio_dev)
		return -ENOMEM;

	adc = iio_priv(indio_dev);
	adc->client = client;

	indio_dev->name = "Hallo!"; //id->name;
	indio_dev->info = &my_adc_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = my_adc_channels;
	indio_dev->num_channels = ARRAY_SIZE(my_adc_channels);

	i2c_set_clientdata(client, indio_dev);

	i2c_smbus_write_byte(client, 0x1);

	return devm_iio_device_register(&client->dev, indio_dev);
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
