#if (defined(RAC5_NTSCU) + defined(RAC5_PAL) + defined(RAC5_NTSCJ)) > 1
#error Select only one RAC5 region
#endif
#ifdef RAC5_AUTO
#error Select RAC5_NTSCU, RAC5_PAL, or RAC5_NTSCJ
#endif

#if defined(RAC5_PAL)
#define GAME_MAP_ID               0x01ede348
#define GAME_MODULE_ID            0x01edc5d0
#define GAME_MODULES              0x01fbe750
#define GAME_LEVEL_TO_LAUNCH      0x01edd8e4
#elif defined(RAC5_NTSCJ)
#define GAME_MAP_ID               0x01ee0068
#define GAME_MODULE_ID            0x01ede4f8
#define GAME_MODULES              0x01fbe790
#define GAME_LEVEL_TO_LAUNCH      0x01edf804
#else
#define GAME_MAP_ID               0x01ede348
#define GAME_MODULE_ID            0x01edc7d0
#define GAME_MODULES              0x01fbe750
#define GAME_LEVEL_TO_LAUNCH      0x01eddae4
#endif
#define GAME_LEVEL_COUNT          25
#define GAME_MODULE_COUNT         8
#define GAME_MODULE_SIZE          0x418

#define FUNCTION(name, address) \
    .set push; .set noreorder; \
    .section .text.name,"ax",@progbits; \
    .align 2; .globl name; .type name,@function; .ent name; \
name: j address; nop; \
    .end name; .size name,.-name; .set pop;


#define VARIABLE_FUNCTION(name, table) \
    .set push; .set noreorder; \
    .section .text.name,"ax",@progbits; \
    .align 2; .globl name; .type name,@function; .ent name; \
name: addiu $sp,$sp,-0xb0; \
    sq $ra,0x00($sp); sq $a0,0x10($sp); sq $a1,0x20($sp); \
    sq $a2,0x30($sp); sq $a3,0x40($sp); \
    sq $8,0x50($sp); sq $9,0x60($sp); sq $10,0x70($sp); sq $11,0x80($sp); \
    swc1 $f12,0x90($sp); swc1 $f13,0x94($sp); \
    swc1 $f14,0x98($sp); swc1 $f15,0x9c($sp); \
    swc1 $f16,0xa0($sp); swc1 $f17,0xa4($sp); \
    swc1 $f18,0xa8($sp); swc1 $f19,0xac($sp); \
    lui $a0,%hi(table); jal GetAddress; addiu $a0,$a0,%lo(table); \
    move $t9,$v0; \
    lq $ra,0x00($sp); lq $a0,0x10($sp); lq $a1,0x20($sp); \
    lq $a2,0x30($sp); lq $a3,0x40($sp); \
    lq $8,0x50($sp); lq $9,0x60($sp); lq $10,0x70($sp); lq $11,0x80($sp); \
    lwc1 $f12,0x90($sp); lwc1 $f13,0x94($sp); \
    lwc1 $f14,0x98($sp); lwc1 $f15,0x9c($sp); \
    lwc1 $f16,0xa0($sp); lwc1 $f17,0xa4($sp); \
    lwc1 $f18,0xa8($sp); lwc1 $f19,0xac($sp); \
    beq $t9,$zero,1f; addiu $sp,$sp,0xb0; \
    jr $t9; nop; \
1:  jr $ra; move $v0,$zero; \
    .end name; .size name,.-name; .set pop;


// Tables contain REL offsets; resolve against the currently loaded module.
.set push
.set noreorder
.section .text.GetAddress,"ax",@progbits
.align 2
.globl GetAddress
.globl GetAddressImmediate
.type GetAddress,@function
.type GetAddressImmediate,@function
.ent GetAddress
GetAddress:
GetAddressImmediate:
    beq $a0,$zero,1f
    move $v0,$zero
    lw $v1,GAME_MAP_ID
    sltiu $t0,$v1,GAME_LEVEL_COUNT
    beq $t0,$zero,1f
    nop
    lw $t0,GAME_LEVEL_TO_LAUNCH
    bne $t0,$v1,1f
    nop
    sll $v1,$v1,2
    addu $v1,$a0,$v1
    lw $v1,0($v1)
    beq $v1,$zero,1f
    nop
    lw $t0,GAME_MODULE_ID
    sltiu $t1,$t0,GAME_MODULE_COUNT
    beq $t1,$zero,1f
    nop
    li $t1,GAME_MODULE_SIZE
    mult $t0,$t1
    mflo $t0
    li $t1,GAME_MODULES
    addu $t0,$t0,$t1
    lw $t1,8($t0)
    andi $t1,$t1,1
    beq $t1,$zero,1f
    nop
    lw $t0,4($t0)
    li $t1,0x00100000
    sltu $t1,$t0,$t1
    bne $t1,$zero,1f
    nop
    li $t1,0x02000000
    sltu $t2,$t0,$t1
    beq $t2,$zero,1f
    nop
    andi $t2,$t0,0x7f
    bne $t2,$zero,1f
    nop
    lw $t2,0($t0)
    li $t3,0x32524e53
    bne $t2,$t3,1f
    nop
    subu $t1,$t1,$t0
    sltu $t1,$v1,$t1
    beq $t1,$zero,1f
    nop
    addu $v0,$t0,$v1
1:
    jr $ra
    nop
.end GetAddress
.size GetAddress,.-GetAddress
.size GetAddressImmediate,.-GetAddressImmediate
.set pop

#if defined(RAC5_PAL)
FUNCTION(gameGetCurrentLevel, 0x01e69840)
FUNCTION(acosf, 0x01e9d7a0)
FUNCTION(asinf, 0x01e9d8a0)
FUNCTION(atan2f, 0x01e9d9a0)
FUNCTION(powf, 0x01e9dec8)
FUNCTION(floorf, 0x01e9d430)
FUNCTION(memset, 0x01ec60e4)
FUNCTION(memcpy, 0x01ec5f3c)
FUNCTION(memmove, 0x01ec5fe8)
FUNCTION(memcmp, 0x01ec5ea8)
FUNCTION(memchr, 0x01ecdf58)
FUNCTION(strlen, 0x01ec66d0)
FUNCTION(strcmp, 0x01ec646c)
FUNCTION(strncmp, 0x01ec69d0)
FUNCTION(strcpy, 0x01ec65b8)
FUNCTION(strncpy, 0x01ec6b98)
FUNCTION(strcat, 0x01ec619c)
FUNCTION(strncat, 0x01ec6818)
FUNCTION(strchr, 0x01ec62cc)
FUNCTION(malloc, 0x01ec81f0)
FUNCTION(free, 0x01ec8240)
FUNCTION(printf, 0x01ec6f28)
FUNCTION(fileOpen, 0x01ebb8d0)
FUNCTION(fileRead, 0x01ebb990)
FUNCTION(fileClose, 0x01ebbaa0)
FUNCTION(sprintf, 0x01ec71c0)
FUNCTION(snprintf, 0x01ec7118)
FUNCTION(atoi, 0x01ec81c8)
FUNCTION(rand, 0x01ec9048)
FUNCTION(soundPlay, 0x01e91638)
FUNCTION(soundPlayAt, 0x01e91960)
FUNCTION(soundStop, 0x01e91a10)
FUNCTION(soundPause, 0x01e91a80)
FUNCTION(soundIsPlaying, 0x01e91b98)
FUNCTION(soundIsLooping, 0x01e91b50)
FUNCTION(soundGet, 0x01e907f0)
FUNCTION(soundGetGlobal, 0x01e906d0)
FUNCTION(soundSetVolume, 0x01e91538)
FUNCTION(soundSetMusicVolume, 0x01e91550)
FUNCTION(soundSetVoiceVolume, 0x01e91570)
FUNCTION(soundPauseAll, 0x01e91af0)
FUNCTION(soundGetSlotCount, 0x01e914e8)
FUNCTION(soundGetSlot, 0x01e91500)
#elif defined(RAC5_NTSCJ)
FUNCTION(gameGetCurrentLevel, 0x01e6b8d0)
FUNCTION(acosf, 0x01e9f6a8)
FUNCTION(asinf, 0x01e9f7a8)
FUNCTION(atan2f, 0x01e9f8a8)
FUNCTION(powf, 0x01e9fdd0)
FUNCTION(floorf, 0x01e9f338)
FUNCTION(memset, 0x01ec7fa4)
FUNCTION(memcpy, 0x01ec7dfc)
FUNCTION(memmove, 0x01ec7ea8)
FUNCTION(memcmp, 0x01ec7d68)
FUNCTION(memchr, 0x01ecfe18)
FUNCTION(strlen, 0x01ec8590)
FUNCTION(strcmp, 0x01ec832c)
FUNCTION(strncmp, 0x01ec8890)
FUNCTION(strcpy, 0x01ec8478)
FUNCTION(strncpy, 0x01ec8a58)
FUNCTION(strcat, 0x01ec805c)
FUNCTION(strncat, 0x01ec86d8)
FUNCTION(strchr, 0x01ec818c)
FUNCTION(malloc, 0x01eca0b0)
FUNCTION(free, 0x01eca100)
FUNCTION(printf, 0x01ec8de8)
FUNCTION(fileOpen, 0x01ebd790)
FUNCTION(fileRead, 0x01ebd850)
FUNCTION(fileClose, 0x01ebd960)
FUNCTION(sprintf, 0x01ec9080)
FUNCTION(snprintf, 0x01ec8fd8)
FUNCTION(atoi, 0x01eca088)
FUNCTION(rand, 0x01ecaf08)
FUNCTION(soundPlay, 0x01e93540)
FUNCTION(soundPlayAt, 0x01e93868)
FUNCTION(soundStop, 0x01e93918)
FUNCTION(soundPause, 0x01e93988)
FUNCTION(soundIsPlaying, 0x01e93aa0)
FUNCTION(soundIsLooping, 0x01e93a58)
FUNCTION(soundGet, 0x01e926f8)
FUNCTION(soundGetGlobal, 0x01e925d8)
FUNCTION(soundSetVolume, 0x01e93440)
FUNCTION(soundSetMusicVolume, 0x01e93458)
FUNCTION(soundSetVoiceVolume, 0x01e93478)
FUNCTION(soundPauseAll, 0x01e939f8)
FUNCTION(soundGetSlotCount, 0x01e933f0)
FUNCTION(soundGetSlot, 0x01e93408)
#else
FUNCTION(gameGetCurrentLevel, 0x01e69a98)
FUNCTION(acosf, 0x01e9d9f8)
FUNCTION(asinf, 0x01e9daf8)
FUNCTION(atan2f, 0x01e9dbf8)
FUNCTION(powf, 0x01e9e120)
FUNCTION(floorf, 0x01e9d688)
FUNCTION(memset, 0x01ec62f4)
FUNCTION(memcpy, 0x01ec614c)
FUNCTION(memmove, 0x01ec61f8)
FUNCTION(memcmp, 0x01ec60b8)
FUNCTION(memchr, 0x01ece168)
FUNCTION(strlen, 0x01ec68e0)
FUNCTION(strcmp, 0x01ec667c)
FUNCTION(strncmp, 0x01ec6be0)
FUNCTION(strcpy, 0x01ec67c8)
FUNCTION(strncpy, 0x01ec6da8)
FUNCTION(strcat, 0x01ec63ac)
FUNCTION(strncat, 0x01ec6a28)
FUNCTION(strchr, 0x01ec64dc)
FUNCTION(malloc, 0x01ec8400)
FUNCTION(free, 0x01ec8450)
FUNCTION(printf, 0x01ec7138)
FUNCTION(fileOpen, 0x01ebbae0)
FUNCTION(fileRead, 0x01ebbba0)
FUNCTION(fileClose, 0x01ebbcb0)
FUNCTION(sprintf, 0x01ec73d0)
FUNCTION(snprintf, 0x01ec7328)
FUNCTION(atoi, 0x01ec83d8)
FUNCTION(rand, 0x01ec9258)
FUNCTION(soundPlay, 0x01e91890)
FUNCTION(soundPlayAt, 0x01e91bb8)
FUNCTION(soundStop, 0x01e91c68)
FUNCTION(soundPause, 0x01e91cd8)
FUNCTION(soundIsPlaying, 0x01e91df0)
FUNCTION(soundIsLooping, 0x01e91da8)
FUNCTION(soundGet, 0x01e90a48)
FUNCTION(soundGetGlobal, 0x01e90928)
FUNCTION(soundSetVolume, 0x01e91790)
FUNCTION(soundSetMusicVolume, 0x01e917a8)
FUNCTION(soundSetVoiceVolume, 0x01e917c8)
FUNCTION(soundPauseAll, 0x01e91d48)
FUNCTION(soundGetSlotCount, 0x01e91740)
FUNCTION(soundGetSlot, 0x01e91758)
#endif

VARIABLE_FUNCTION(msgstr, vaMsgStr)
VARIABLE_FUNCTION(mobyCreate, vaMobyCreate)
VARIABLE_FUNCTION(mobyDestroy, vaMobyDestroy)
