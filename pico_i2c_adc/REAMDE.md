# Raspberry Pi Pico I2C ADC

Ein einfacher Raspberry Pi Pico I2C ADC. Über I2C kann die analoge Spannung an ADC0 (GP26)
ausgelesen werden. Der ADC verwendet die I2C Adresse 0x17. Ein Schreibzugriff
mit den Wert 1 führt dazu, dass der ADC zyklisch gelesen wird. Der zuletzt
gelesene Wert kann über einen I2C Lesezugriff erhalten werden. Ein
Schreibzugriff mit dem Wert 2 führt dazu, dass der ADC einmalig ausgelesen wird.

# Benötigte Software zum Bauen des Beispiels

- cmake
- make
- arm-none-eabi-gcc

# Bauen der Software

Das Raspberry Pi Pico SDK wird zum Bauen benötigt.

~~~
cd
git clone https://github.com/raspberrypi/pico-sdk
~~~

Zum Bauen muss die Umgebunsvariable *PICO_SDK_PATH* gesetzt werden:

~~~
export PICO_SDK_PATH=/home/$USER/pico-sdk
~~~

Anschließend kann der Pico I2C ADC mit folgenden Befehlen gebaut werden.

~~~
mkdir build
cd build
cmake ..
make
~~~
