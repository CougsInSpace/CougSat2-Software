/**
 * CAN Sender Example for RP2040
 * Sends integer 50 to receiver, waits for doubled value (100) back
 * Uses can2040 library: https://github.com/KevinOConnor/can2040
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"  // For get_core_num() alternative
#include "can2040/can2040.h"

// CAN message IDs
#define CAN_ID_SEND     0x100  // Sender -> Receiver (carries original number)
#define CAN_ID_RESPONSE 0x200  // Receiver -> Sender (carries doubled number)

// CAN timing for 500kbps (adjust for your transceiver)
#define CAN_BITRATE     500000

// GPIO pins for CAN transceiver
#define CAN_GPIO_TX     5   // Connect to CAN transceiver TX
#define CAN_GPIO_RX     4  // Connect to CAN transceiver RX

// Global CAN structure
static struct can2040 cbus;

static void
PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

// Callback function for CAN events
static void can2040_callback(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (notify == CAN2040_NOTIFY_RX) {
        // Check if it's a response message
        if (msg->id == CAN_ID_RESPONSE && msg->dlc >= 4) {
            // Reconstruct the 32-bit integer from data bytes
            int32_t received_value = msg->data[0] |
                                     (msg->data[1] << 8) |
                                     (msg->data[2] << 16) |
                                     ((int32_t)msg->data[3] << 24);
            
            printf("Received doubled value: %ld\n", received_value);
            
            if (received_value == 100) {
                printf("SUCCESS: 50 doubled to 100!\n");
            } else {
                printf("ERROR: Expected 100, got %ld\n", received_value);
            }
        }
    }
}

// Setup CAN interface
static void can_setup(void)
{
    uint32_t pio_num =0;
    uint32_t sys_clock = SYS_CLK_HZ;  // RP2040 default clock
    uint32_t bitrate = CAN_BITRATE;
    uint32_t gpio_rx = CAN_GPIO_RX;
    uint32_t gpio_tx = CAN_GPIO_TX;
    // Initialize can2040 module
    can2040_setup(&cbus, pio_num);
    
    // Register callback
    can2040_callback_config(&cbus, can2040_callback);
    

    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, 1);
    irq_set_enabled(PIO0_IRQ_0, 1);
    // Start CAN on core 0 (main core)
    // The second parameter is the PIO block number (0 or 1)
    // On RP2040, core 0 uses PIO 0, core 1 uses PIO 1
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);  // Use PIO 0 for core 0
}

// Send a 32-bit integer over CAN
int can_send_int(uint32_t id, int32_t value)
{
    struct can2040_msg msg;
    
    msg.id = id;
    msg.dlc = 4;

    msg.data[0] = value & 0xFF;
    msg.data[1] = (value >> 8) & 0xFF;
    msg.data[2] = (value >> 16) & 0xFF;
    msg.data[3] = (value >> 24) & 0xFF;
    
    return can2040_transmit(&cbus, &msg);
}

int main()
{
    // Initialize stdio for debug output
    stdio_init_all();
    sleep_ms(2000);  // Wait for USB serial to connect
    printf("CAN Sender starting...\n");
    
    // Setup CAN
    can_setup();
    printf("CAN initialized at %d bps\n", CAN_BITRATE);
    
    // Main loop
    while (true) {
        // Send the number 50 every 2 seconds
        int32_t value_to_send = 50;
        
        printf("Sending value: %ld\n", value_to_send);
        
        if (can_send_int(CAN_ID_SEND, value_to_send) == 0) {
            printf("Message sent successfully\n");
        } else {
            printf("Failed to send message\n");
        }
        
        // Wait before sending again
        sleep_ms(2000);
    }
    
    return 0;
}