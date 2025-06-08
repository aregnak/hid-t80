#include "hid-t80.h"

int t80_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
    int ret;

    pr_info("T80 HID driver: probe()\n");

    ret = hid_parse(hdev);
    if (ret) {
        pr_err("T80: Failed to parse HID device\n");
        return ret;
    }

    ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
    if (ret) {
        pr_err("T80: Failed to start HID hardware\n");
        return ret;
    }

    return 0;
}

static int t80_raw_event(struct hid_device *hdev, struct hid_report *report, u8 *data, int size)
{
    int i;

    pr_info("T80 raw report (%d bytes):", size);
    for (i = 0; i < size; i++)
        pr_cont(" %02x", data[i]);
    pr_cont("\n");

    return 0;
}

static void t80_remove(struct hid_device *hdev)
{
    pr_info("T80 HID driver: remove()\n");
    hid_hw_stop(hdev);
}

static struct hid_driver t80_driver = {
    .name = "t80_hid_driver",
    .id_table = t80_devices,
    .probe = t80_probe,
    .remove = t80_remove,
    .raw_event = t80_raw_event,
};
module_hid_driver(t80_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Areg Nakashian");
MODULE_DESCRIPTION("HID driver for Thrustmaster T80 Racing Wheel");