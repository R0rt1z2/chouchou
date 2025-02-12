#include "include/printf.h"
#include "include/string.h"
#include "include/fastboot.h"
#include "include/common.h"
#include "include/commands.h"

#define VERSION "0.1"

void original_flash(const char *arg, void *data, unsigned sz) {
    ((void (*)(const char *arg, void *data, unsigned sz))(0x4c436a18 | 1))(arg, data, sz);
}

void cmd_hexdump(const char *arg, void *data, unsigned sz) {
    if (!arg || !*arg) {
        fastboot_fail("Usage: hexdump <addr> <size>");
        return;
    }

    // TODO: Do better memory management
    char args[128];
    strncpy(args, arg, sizeof(args) - 1);
    args[sizeof(args) - 1] = '\0';

    char *a = strtok(args, " ");
    char *s = strtok(NULL, " ");

    if (a && s) {
        long al = strtol(a, NULL, 0);
        long sl = strtol(s, NULL, 0);

        if (al == 0 || sl <= 0) {
            fastboot_fail("Invalid address or size");
            return;
        }

        video_hexdump((void *)al, sl);
    } else {
        fastboot_fail("Usage: hexdump <addr> <size>");
    }

    fastboot_okay("");
}

void cmd_help(const char *arg, void *data, unsigned sz) {
    fastboot_info("\nAvailable oem commands:");
    struct fastboot_cmd *cmd = get_fastboot_cmd_list();
    while (cmd) {
        if (cmd->prefix) {
            if (strncmp(cmd->prefix, "oem", 3) == 0) {
                fastboot_info(cmd->prefix);
            }
        }
        cmd = cmd->next;
    }
    fastboot_okay("");
}

void cmd_flash(const char *arg, void *data, unsigned sz) {
    if (!arg || *arg == '\0') {
        fastboot_fail("Invalid argument!");
        return;
    }

    // TODO: Handle protected partitions properly
    if (strcmp(arg, "misc") == 0 || 
        strcmp(arg, "boot1") == 0 || 
        strcmp(arg, "boot2") == 0 || 
        strcmp(arg, "preloader") == 0 ||
        strcmp(arg, "preloader_a") == 0 ||
        strcmp(arg, "preloader_b") == 0) {
        fastboot_fail("Partition is protected");
        return;
    }

    original_flash(arg, data, sz);
}

void cmd_flashing_lock(const char *arg, void *data, unsigned sz) {
    fastboot_info("");
    fastboot_info("To lock the bootloader, you need to flash");
    fastboot_info("stock firmare through SP Flash Tool first");
    fastboot_info("");
    fastboot_fail(":(");
}

void cmd_version(const char *arg, void *data, unsigned sz) {
    char buffer[64];
    const char *prefix = "ChouChou version: ";

    strcpy(buffer, prefix);
    strcat(buffer, VERSION);

    fastboot_info(buffer);
    fastboot_okay("");
}

void register_commands() {
    fastboot_register("oem version", cmd_version, 1);
    fastboot_register("oem hexdump", cmd_hexdump, 1);
    fastboot_register("oem help", cmd_help, 1);
    fastboot_register("flash:", cmd_flash, 1);
    fastboot_register("flashing lock", cmd_flashing_lock, 1);

    LOGD("Successfully registered fastboot commands!\n");
}