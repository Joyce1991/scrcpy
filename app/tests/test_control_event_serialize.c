#include <assert.h>

#include "controlevent.h"

static void test_serialize_keycode_event() {
    struct control_event event = {
        .type = CONTROL_EVENT_TYPE_KEYCODE,
        .keycode_event = {
            .action = AKEY_EVENT_ACTION_UP,
            .keycode = AKEYCODE_ENTER,
            .metastate = AMETA_SHIFT_ON | AMETA_SHIFT_LEFT_ON,
        },
    };

    unsigned char buf[SERIALIZED_EVENT_MAX_SIZE];
    int size = control_event_serialize(&event, buf);
    assert(size == 10);

    const unsigned char expected[] = {
        0x00, // CONTROL_EVENT_TYPE_KEYCODE
        0x01, // AKEY_EVENT_ACTION_UP
        0x00, 0x00, 0x00, 0x42, // AKEYCODE_ENTER
        0x00, 0x00, 0x00, 0x41, // AMETA_SHIFT_ON | AMETA_SHIFT_LEFT_ON
    };
    assert(!memcmp(buf, expected, sizeof(expected)));
}

static void test_serialize_text_event() {
    struct control_event event = {
        .type = CONTROL_EVENT_TYPE_TEXT,
        .text_event = {
            .text = "hello, world!",
        },
    };

    unsigned char buf[SERIALIZED_EVENT_MAX_SIZE];
    int size = control_event_serialize(&event, buf);
    assert(size == 15);

    const unsigned char expected[] = {
        0x01, // CONTROL_EVENT_TYPE_KEYCODE
        0x0d, // text length
        'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', // text
    };
    assert(!memcmp(buf, expected, sizeof(expected)));
}

static void test_serialize_mouse_event() {
    struct control_event event = {
        .type = CONTROL_EVENT_TYPE_MOUSE,
        .mouse_event = {
            .action = AMOTION_EVENT_ACTION_DOWN,
            .buttons = AMOTION_EVENT_BUTTON_PRIMARY,
            .x = 260,
            .y = 1026,
        },
    };

    unsigned char buf[SERIALIZED_EVENT_MAX_SIZE];
    int size = control_event_serialize(&event, buf);
    assert(size == 14);

    const unsigned char expected[] = {
        0x02, // CONTROL_EVENT_TYPE_MOUSE
        0x00, // AKEY_EVENT_ACTION_DOWN
        0x00, 0x00, 0x00, 0x01, // AMOTION_EVENT_BUTTON_PRIMARY
        0x00, 0x00, 0x01, 0x04, // 260
        0x00, 0x00, 0x04, 0x02, // 1026
    };
    assert(!memcmp(buf, expected, sizeof(expected)));
}

static void test_serialize_scroll_event() {
    struct control_event event = {
        .type = CONTROL_EVENT_TYPE_SCROLL,
        .scroll_event = {
            .x = 260,
            .y = 1026,
            .hscroll = 1,
            .vscroll = -1,
        },
    };

    unsigned char buf[SERIALIZED_EVENT_MAX_SIZE];
    int size = control_event_serialize(&event, buf);
    assert(size == 17);

    const unsigned char expected[] = {
        0x03, // CONTROL_EVENT_TYPE_SCROLL
        0x00, 0x00, 0x01, 0x04, // 260
        0x00, 0x00, 0x04, 0x02, // 1026
        0x00, 0x00, 0x00, 0x01, // 1
        0xFF, 0xFF, 0xFF, 0xFF, // -1
    };
    assert(!memcmp(buf, expected, sizeof(expected)));
}

int main() {
    test_serialize_keycode_event();
    test_serialize_text_event();
    test_serialize_mouse_event();
    test_serialize_scroll_event();
    return 0;
}
