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

    // Set up proper racing wheel axes
    input_set_abs_params(input, ABS_X, 0, 65535, 0, 0);
    input_set_abs_params(input, ABS_Y, 65535, 0, 0, 0);
    input_set_abs_params(input, ABS_Z, 65535, 0, 0, 0);

    // Enable necessary event types
    __set_bit(EV_ABS, input->evbit);
    __set_bit(EV_KEY, input->evbit);

    // Buttons
    __set_bit(BTN_A, input->keybit);
    __set_bit(BTN_B, input->keybit);

    hid_info(hdev, "T80 racing wheel configured\n");
    return 0;
}

static int t80_raw_event(struct hid_device *hdev, struct hid_report *report,
                        u8 *data, int size)
{
    struct hid_input *hidinput;
    struct input_dev *input;
    u16 steering, gas, brake;
    u8 buttons;

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
    steering = (s16)((data[44] << 8) | data[43]);
    gas = (data[46] << 8) | data[45];     // Full 16-bit throttle
    brake = (data[48] << 8) | data[47];   // Full 16-bit brake
    buttons  = data[6];

    // Send input events 
    input_report_abs(input, ABS_X, steering);
    input_report_abs(input, ABS_Y, gas);
    input_report_abs(input, ABS_Z, brake);
    input_sync(input);

    return 0;
}

static struct hid_driver t80_driver = {
    .name = "t80",
    .id_table = t80_devices,
    .input_configured = t80_input_configured,
    .raw_event = t80_raw_event,
};

module_hid_driver(t80_driver);