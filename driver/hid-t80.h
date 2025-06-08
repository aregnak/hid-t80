#ifndef HID_T80_H
#define HID_T80_H

#include <linux/hid.h>
#include <linux/input.h>

#define VENDOR_ID 0x044f
#define PRODUCT_ID 0xb668

static const struct hid_device_id t80_devices[] = 
{
    { HID_USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    { }
};
MODULE_DEVICE_TABLE(hid, t80_devices);

#endif