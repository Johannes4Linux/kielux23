#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <hardware/i2c.h>
#include <pico/i2c_slave.h>

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 100000; // 100 kHz

static const uint I2C_SLAVE_SDA_PIN = 16;
static const uint I2C_SLAVE_SCL_PIN = 17;

bool single_shot = false;
bool read_continuously = false;
uint8_t adc_data = 0x0;

// Our handler is called from the I2C ISR, so it must complete quickly. Blocking calls /
// printing to stdio may interfere with interrupt handling.
static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
	uint8_t value;
	switch (event) {
		case I2C_SLAVE_RECEIVE: // master has written some data
			value = i2c_read_byte_raw(i2c);
			single_shot = value == 2;
			read_continuously = value == 1;
			break;
		case I2C_SLAVE_REQUEST: // master is requesting data
			i2c_write_byte_raw(i2c, adc_data);
			break;
		default:
			break;
	}
}

int main() {
	stdio_init_all();
	printf("ADC Example, measuring GPIO26\n");

	adc_init();

	// Make sure GPIO is high-impedance, no pullups etc
	adc_gpio_init(26);
	// Select ADC input 0 (GPIO26)
	adc_select_input(0);

	// Init I2C
	gpio_init(I2C_SLAVE_SDA_PIN);
	gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SLAVE_SDA_PIN);

	gpio_init(I2C_SLAVE_SCL_PIN);
	gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_SLAVE_SCL_PIN);

	i2c_init(i2c0, I2C_BAUDRATE);
	// configure I2C0 for slave mode
	i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);
	

	while (1) {
		printf("single: %d, cont: %d\n", single_shot, read_continuously);
		if((single_shot == true) || (read_continuously == true) ) {
			// 12-bit conversion, assume max value == ADC_VREF == 3.3 V
			const float conversion_factor = 3.3f / (1 << 12);
			uint16_t result = adc_read();
			adc_data = result >> 4;
			printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
			if(single_shot == true)
				single_shot = false;
		}
		sleep_ms(500);
		
	}
}

