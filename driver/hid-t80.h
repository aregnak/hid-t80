#ifndef HID_T80_H
#define HID_T80_H

#include <linux/hid.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define VENDOR_ID 0x044f // Thrustmaster
#define PRODUCT_ID 0xb668 // T80

static const struct hid_device_id t80_devices[] = 
{ 
    { HID_USB_DEVICE(VENDOR_ID, PRODUCT_ID) }, 
    {} 
}; 

MODULE_DEVICE_TABLE(hid, t80_devices);

static int t80_probe(struct hid_device *hdev, const struct hid_device_id *id);
static int t80_raw_event(struct hid_device *hdev, struct hid_report *report, u8 *data, int size);
static void t80_remove(struct hid_device *hdev);

#endif // HID_T80_H