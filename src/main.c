#include <stddef.h>
#include <stdint.h>

#include "irq.h"
#include "mini_uart.h"
#include "printf.h"
#include "sched.h"
#include "task.h"
#include "timer.h"
#include "utils.h"
#include "mm.h"
#include "sd.h"
#include "debug.h"
#include "loader.h"

void hypervisor_main() {
  uart_init();
  init_printf(NULL, putc);
  printf("=== raspvisor ===\n");

  init_task_console(current);
  init_initial_task();
  irq_vector_init();
  timer_init();
  disable_irq();
  enable_interrupt_controller();

  if (sd_init() < 0)
    PANIC("sd_init() failed.");

  struct raw_binary_loader_args bl_args1 = {
    .load_addr = 0x0,
    .entry_point = 0x0,
    .sp = 0x100000,
    .filename = "mini-os.bin",
  };
  if (create_task(raw_binary_loader, &bl_args1) < 0) {
    printf("error while starting task");
    return;
  }

  struct raw_binary_loader_args bl_args2 = {
    .load_addr = 0x0,
    .entry_point = 0x0,
    .sp = 0x100000,
    .filename = "echo.bin",
  };
  if (create_task(raw_binary_loader, &bl_args2) < 0) {
    printf("error while starting task");
    return;
  }

  struct raw_binary_loader_args bl_args3 = {
    .load_addr = 0x0,
    .entry_point = 0x0,
    .sp = 0x100000,
    .filename = "mini-os.bin",
  };
  if (create_task(raw_binary_loader, &bl_args3) < 0) {
    printf("error while starting task #2");
    return;
  }

  struct raw_binary_loader_args bl_args4 = {
    .load_addr = 0x0,
    .entry_point = 0x0,
    .sp = 0x100000,
    .filename = "echo.bin",
  };
  if (create_task(raw_binary_loader, &bl_args4) < 0) {
    printf("error while starting task");
    return;
  }

  struct raw_binary_loader_args bl_args5 = {
    .load_addr = 0x0,
    .entry_point = 0x0,
    .sp = 0x100000,
    .filename = "mini-os.bin",
  };
  if (create_task(raw_binary_loader, &bl_args5) < 0) {
    printf("error while starting task");
    return;
  }

  while (1) {
    disable_irq();
    schedule();
    enable_irq();
  }
}
