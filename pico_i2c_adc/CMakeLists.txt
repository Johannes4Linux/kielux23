cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(i2c_adc
    main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(i2c_adc
	pico_stdlib
	hardware_adc
	pico_i2c_slave
	hardware_i2c
	)

pico_enable_stdio_usb(i2c_adc 1)
pico_enable_stdio_uart(i2c_adc 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(i2c_adc)
