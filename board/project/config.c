#include "config.h"

config_t *config_read()
{
    uint16_t *version = (uint16_t *)(XIP_BASE + CONFIG_FLASH_TARGET_OFFSET);
    if (*version != CONFIG_VERSION)
    {
        config_forze_write();
    }
    return (config_t *)(XIP_BASE + CONFIG_FLASH_TARGET_OFFSET);
}

void config_write(config_t *config)
{
    uint8_t flash[FLASH_PAGE_SIZE] = {0};
    memcpy(flash, (uint8_t *)config, sizeof(config_t));

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(CONFIG_FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(CONFIG_FLASH_TARGET_OFFSET, flash, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
}

void config_get(config_t *config)
{
    memcpy(config, (config_t *)(XIP_BASE + CONFIG_FLASH_TARGET_OFFSET), sizeof(config_t));
    config->debug = MSRC_DEBUG;
}

void config_forze_write()
{
    config_t config;
    config.version = CONFIG_VERSION;
    config.esc_protocol = ESC_PROTOCOL;
    config.alpha_current = AVERAGING_ELEMENTS_CURRENT;
    config.pairOfPoles = RPM_PAIR_OF_POLES;
    config.debug = MSRC_DEBUG;
    config.spare1 = 0;
    config.spare2 = 0;
    config.spare3 = 0;
    config.spare4 = 0;
    config.spare5 = 0;

    config_write(&config);
}
