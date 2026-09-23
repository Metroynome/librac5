#ifndef LIBRAC5_UI_H
#define LIBRAC5_UI_H

#include <tamtypes.h>
#include <librac5/types.h>

#define UI_CHOOSE_PROFILE_START (0x0108FB00)

typedef struct UiElement_Select { // 0xe8
/* 0x00 */ int stringId;
/* 0x04*/ char unk_04[0xc8];
/* 0xcc */ bool enable;
/* 0xcd */ bool display;
/* 0xce */ char pad_ce[0x2];
/* 0xd0 */ char unk_d0[0x18];
}UiElement_Select_t;

typedef struct UiMenu {
/* 0x00 */ int *pChildren;
/* 0x04 */ int numChildren;
/* 0x08 */ int selectedIndex;
/* 0x0c */ float defaultTextSize;
/* 0x10 */ float selectedTextSize;
/* 0x14 */ u8 flag;  // 0x00 = default, shows option background always, 0x01: Only show selected option background, 0x02: hide all text and background
/* 0x15 */ char pad_15[0x3];
/* 0x18 */ u32 selectedBorderColor;
/* 0x1c */ float lerpBackgroundColor;
} UiMenu_t;


typedef struct uiProfile { // 0x5f8
/* 0x000 */ char unk_000[0x5f8];
} uiProfile_t;
#endif
