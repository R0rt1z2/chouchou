#include "include/arm.h"
#include "include/commands.h"
#include "include/common.h"
#include "include/printf.h"

typedef enum
{    
    DISP_MODULE_OVL0  ,
    DISP_MODULE_OVL1  ,
    DISP_MODULE_RDMA0 ,
    DISP_MODULE_RDMA1 ,
    DISP_MODULE_WDMA0 ,
    DISP_MODULE_COLOR0,
    DISP_MODULE_CCORR ,
    DISP_MODULE_AAL   ,
    DISP_MODULE_GAMMA ,
    DISP_MODULE_DITHER,
    DISP_MODULE_UFOE  , //10
    DISP_MODULE_PWM0   ,
    DISP_MODULE_WDMA1 ,
    DISP_MODULE_DSI0  ,
    DISP_MODULE_DPI0   ,
    DISP_MODULE_DPI1 ,
    DISP_MODULE_SMI,
    DISP_MODULE_CONFIG,
    DISP_MODULE_CMDQ,
    DISP_MODULE_MUTEX,	

    DISP_MODULE_COLOR1,
    DISP_MODULE_RDMA2,
    DISP_MODULE_PWM1,
    DISP_MODULE_OD,
    DISP_MODULE_MERGE,
    DISP_MODULE_SPLIT0,
    DISP_MODULE_SPLIT1,
    DISP_MODULE_DSI1,
    DISP_MODULE_DSIDUAL,    

	DISP_MODULE_SMI_LARB0 ,
    DISP_MODULE_SMI_COMMON,
    DISP_MODULE_UNKNOWN, //20
    DISP_MODULE_NUM
} DISP_MODULE_ENUM;

int app() {
    return ((int (*)(void))(0x4c42b790 | 1))();
}

void platform_init() {
    ((void (*)(void))(0x4c404864 | 1))();
}

void DSI_set_cmdq(DISP_MODULE_ENUM module, void* cmdq, unsigned int *pdata, unsigned int queue_size, unsigned char force_update) {
    dprintf("SHIM: module=%d, cmdq=%p, pdata=%p, queue_size=%d, force_update=%d\n", module, cmdq, pdata, queue_size, force_update);
    ((void (*)(DISP_MODULE_ENUM, void*, unsigned int*, unsigned int, unsigned char))(0x4c415188 | 1))(module, cmdq, pdata, queue_size, force_update);
}

void late_init() {
    register_commands();

    volatile uint16_t *x = (volatile uint16_t *)0x4c44f0c0; // orange_state_warning
    x[0] = 0x2000; // movs r0, #0
    x[1] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)0x4c44f0c0, 2);

    x = (volatile uint16_t *)0x4c44eff0; // red_state_warning
    x[0] = 0x4770; // bx lr
    arch_clean_invalidate_cache_range((uint32_t)0x4c44eff0, 2);
}

void early_init() {
    patch_call(0x4c415388, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c415388, 4);
    patch_call(0x4c4153d6, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c4153d6, 4);
    patch_call(0x4c4153f2, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c4153f2, 4);
    patch_call(0x4c41540c, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c41540c, 4);
    patch_call(0x4c415424, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c415424, 4);
    patch_call(0x4c41543c, (void *)DSI_set_cmdq, JUMP_BL);
    arch_clean_invalidate_cache_range(0x4c41543c, 4);

    __asm__("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
    ((volatile uint32_t *)0x4c5207c8)[0] = BOOT_STATE_GREEN;
    arch_clean_invalidate_cache_range(0x4c5207c8, 4);

    volatile uint16_t *x = (volatile uint16_t *)0x4c42c8e0; // register cmd_flash
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)0x4c42c8e0, 2);

    x = (volatile uint16_t *)0x4c42cb72; // register cmd_flashing_lock
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)0x4c42cb72, 2);

    x = (volatile uint16_t *)0x4c42c8f4; // register cmd_erase
    x[0] = 0x46c0; // nop
    x[1] = 0x46c0; // nop
    arch_clean_invalidate_cache_range((uint32_t)0x4c42c8f4, 2);
}