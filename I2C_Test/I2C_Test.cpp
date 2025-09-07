#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#define I2C_PORT i2c0 // Define which I2C port to use
#define I2C_SDA_PIN 16   // Define SDA pin for I2C
#define I2C_SCL_PIN 17  // Define SCL pin for I2C
#define I2C_BUS_ID 0x68 
#define I2C_CLOCK 400000 //I2C clock speed, could be upped if necessary, 400khz

void writeBrightness(int brightness, int ledID){
    
    int range= 6+((ledID-1)*4); // Calculate the register address for the LED based on ledID, each LED takes 4 bytes in the register.
    int rangeEnd = range + 4; // End of the register range for the LED
    // Print the starting register address for debugging purposes
    uint8_t brightnessByte = (uint8_t)brightness;//convert to uint8_t from int 
    for(range; range < rangeEnd; range++){
        // Write the brightness value to each register for the LED
        //printf("\n%d", &range);
        //printf("%d\n", range); 
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)&range, 1, true); // Send the register address to write to
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &brightnessByte, 1, false); // Send the brightness value
    }
}
int readLightSensor(int diodeNum){
    adc_select_input(diodeNum); // Select the ADC input for the specified diode number (0-3)
    return adc_read(); // Read the ADC value from the selected input
}
int main()
{
    stdio_init_all();
    //init the i2c bus
    adc_init();
    i2c_init(I2C_PORT, I2C_CLOCK); // Initialize I2C with the specified clock speed
    //set up analog pins
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    //config the i2c and make it pull up 
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C); // Set SDA pin to I2C function
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C); // Set SCL pin to I2C function
    gpio_pull_up(I2C_SDA_PIN); // Enable pull-up resistor on SDA pin
    gpio_pull_up(I2C_SCL_PIN); // Enable pull-up resistor on SCL pin
    //setup heartbeat led 
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //do config for the led breakout  board 
    //write to the board at a specific register
    //then you can write what you actually want changed at that register lmao
    uint8_t i2c_Value = 0b10101010;//10 sets each individual led to pwm control, page 33 of tlc59116 datasheet, this is the register value to set all LEDs to PWM control. You can change this value to whatever you want for your application.
    //write to leds
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x14, 1, true);
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value,1, false); 

    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x15, 1, true);
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value,1, false); 
     
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x16, 1, true);
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value,1, false); 
    
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x17, 1, true);
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value,1, false); 
    
    i2c_Value=0;//sets group pwm control brightness control to true
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x12, 1, true);
    i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value,1, false); 
    

    while (true) {
        //blink led
        gpio_put(PICO_DEFAULT_LED_PIN, true);
        sleep_ms(250);
        gpio_put(PICO_DEFAULT_LED_PIN, false);
        sleep_ms(250);
        
        gpio_init(18);
        gpio_set_dir(18, GPIO_OUT); 
        gpio_put(18,1);//writes 13 to high, enables system. 

        i2c_Value=0b00000001;//turn on the oscillator, set to all call bus 
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x00, 1, true); 
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value, 1, false);

        i2c_Value=0b00000000; //setup 2nd mode register
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, (uint8_t*)0x01,1,true);
        i2c_write_blocking(I2C_PORT, I2C_BUS_ID, &i2c_Value, 1, false); 

        writeBrightness(255, 0);//test brightness
        printf("\n");
        writeBrightness(255, 1);//test brightness for LED 2
        printf("\n");
        writeBrightness(255, 2);//test brightness for LED 3
        printf("\n");
        writeBrightness(255,3);
        int lightReading0 = readLightSensor(0); // Read from the first light sensor (diode 1)
        printf("Light Reading 0:\n"); 
        printf("%i \n",lightReading0);

        int lightReading1 = readLightSensor(1); // Read from the first light sensor (diode 1)
        printf("Light Reading 1:\n"); 
        printf("%i \n",lightReading1);

        int lightReading2 = readLightSensor(2); // Read from the first light sensor (diode 1)
        printf("Light Reading 2:\n"); 
        printf("%i \n",lightReading2);
        
        sleep_ms(1000);
    }
}
