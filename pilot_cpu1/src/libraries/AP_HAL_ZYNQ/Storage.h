#pragma once

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_PX4_Namespace.h"
#include "FreeRTOS.h"

#define PX4_STORAGE_SIZE HAL_STORAGE_SIZE
#define PX4_STORAGE_MAX_WRITE 512
#define PX4_STORAGE_LINE_SHIFT 9
#define PX4_STORAGE_LINE_SIZE (1<<PX4_STORAGE_LINE_SHIFT)
#define PX4_STORAGE_NUM_LINES (PX4_STORAGE_SIZE/PX4_STORAGE_LINE_SIZE)

class PX4::PX4Storage : public AP_HAL::Storage {
public:
    PX4Storage();

    void init() {}
    void read_block(void *dst, uint16_t src, size_t n);
    void write_block(uint16_t dst, const void* src, size_t n);

    void _timer_tick(void);

private:
    int _fd;
    volatile bool _initialised;
    void _storage_create(void);
    void _storage_open(void);
    xSemaphoreHandle dirty_sem; 
    void _mark_dirty(uint16_t loc, uint16_t length);
    uint8_t _buffer[PX4_STORAGE_SIZE] __attribute__((aligned(4)));
    volatile uint32_t _dirty_mask;
    bool _have_mtd;
    void _upgrade_to_mtd(void);
    uint32_t _mtd_signature(void);
    void _mtd_write_signature(void);

    irqstate_t irq_state;
    void bus_lock(bool lock);
};
