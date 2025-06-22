#include <linux/input-event-codes.h>
#include <linux/module.h>
#include <linux/hid.h>
#include <linux/input.h>
#include "hid-t80.h"

MODULE_AUTHOR("Areg Nakashian");
MODULE_DESCRIPTION("Thrustmaster T80 Wheel Driver");
MODULE_LICENSE("GPL");

static int t80_input_configured(struct hid_device *hdev, struct hid_input *hidinput)
{
    struct input_dev *input = hidinput->input;

    // Clear existing EV_ABS settings 
    __clear_bit(EV_ABS, input->evbit);
    memset(input->evbit, 0, sizeof(input->evbit));
    memset(input->absbit, 0, sizeof(input->absbit));
    //memset(input->keybit, 0, sizeof(input->keybit));

    // Set up proper racing wheel axes
    input_set_abs_params(input, ABS_X, 0, 65535, 0, 0);
    input_set_abs_params(input, ABS_Y, 65535, 0, 0, 0);
    input_set_abs_params(input, ABS_Z, 65535, 0, 0, 0);

    // Enable necessary event types
    __set_bit(EV_ABS, input->evbit);
    __set_bit(EV_KEY, input->evbit);

    // Buttons
    // The comments next to each mapping is wrong, I'll fix them later.
    __set_bit(BTN_SOUTH,  input->keybit); // Cross
    __set_bit(BTN_EAST,   input->keybit); // Circle
    __set_bit(BTN_WEST,   input->keybit); // Square
    __set_bit(BTN_NORTH,  input->keybit); // Triangle

    __set_bit(BTN_TL,     input->keybit); // Shift down
    __set_bit(BTN_TR,     input->keybit); // Shift up
    __set_bit(BTN_TL2,    input->keybit); // L2
    __set_bit(BTN_TR2,    input->keybit); // R2

    __set_bit(BTN_THUMBL, input->keybit); // L3
    __set_bit(BTN_THUMBR, input->keybit); // R3

    __set_bit(BTN_START,  input->keybit); // Start

    __set_bit(BTN_DPAD_UP,    input->keybit); // D-pad Up
    __set_bit(BTN_DPAD_DOWN,  input->keybit); // D-pad Down

    hid_info(hdev, "T80 racing wheel configured\n");
    return 0;
}

static int t80_raw_event(struct hid_device *hdev, struct hid_report *report,
                        u8 *data, int size)
{
    struct hid_input *hidinput;
    struct input_dev *input;
    u16 steering, gas, brake;

    // Basic sanity checks
    if (size < 49)
        return 0;

    if (list_empty(&hdev->inputs)) {
        hid_err(hdev, "no inputs found\n");
        return -ENODEV;
    }

    // Get the first input device
    hidinput = list_first_entry(&hdev->inputs, struct hid_input, list);
    input = hidinput->input;

    // Parse steering and pedals - manual little-endian conversion
    // data[43]-data[48] are all analog outputs, steering, gas, and brake respectively
    steering = (s16)((data[44] << 8) | data[43]);
    gas = (data[46] << 8) | data[45];     // Full 16-bit throttle
    brake = (data[48] << 8) | data[47];   // Full 16-bit brake

    // Send input events 
    input_report_abs(input, ABS_X, steering);
    input_report_abs(input, ABS_Y, gas);
    input_report_abs(input, ABS_Z, brake);

    // data[5]-data[9] are all button outputs
    u8 b5 = data[5], b6 = data[6], b7 = data[7], b8 = data[8], b9 = data[9];

    // Face buttons
    input_report_key(input, BTN_EAST, b5 & 0x20); // Cross
    input_report_key(input, BTN_TL,  b5 & 0x40); // Circle
    input_report_key(input, BTN_SOUTH,  b5 & 0x10); // Square
    input_report_key(input, BTN_NORTH, b5 & 0x80); // Triangle

    // Shift paddles
    input_report_key(input, BTN_WEST,    b6 & 0x01); // Shift down
    input_report_key(input, BTN_TR,    b6 & 0x02); // Shift up

    // L2 & R2 buttons
    input_report_key(input, BTN_TL2,   b6 & 0x04); // L2
    input_report_key(input, BTN_TR2,   b6 & 0x08); // R2

    // Back buttons
    input_report_key(input, BTN_THUMBL, (b6 & 0x40) && (b8 == 0xff)); // L3
    input_report_key(input, BTN_THUMBR, (b6 & 0x80) && (b9 == 0xff)); // R3

    // Start button
    input_report_key(input, BTN_START, b7 & 0x01);

    // D-Pad ?
    input_report_key(input, BTN_DPAD_UP,    b6 & 0x20);
    input_report_key(input, BTN_DPAD_DOWN,  b6 & 0x10);
    input_sync(input);

    return 1;
}

static struct hid_driver t80_driver = {
    .name = "t80",
    .id_table = t80_devices,
    .input_configured = t80_input_configured,
    .raw_event = t80_raw_event,
};

module_hid_driver(t80_driver);