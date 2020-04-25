/*
 *      Author: sc lee
 *
 *
 *      Demo of use PDM and Sound under Mbed
 *
 *      Inorder to run this demo, you should have CY8CKIT-062-WIFI-BT+TFT with CY8CKIT-028-TFT
 *
 *      Modifiy the aws_config.h before run
 *
 *		Detail:
 *
 *
 * Licensed under the Apache License, Version 2.0
 */




#include "mbed.h"
#include "sound.h"


#if defined(HW_STATS)
#if !defined(MBED_CPU_STATS_ENABLED) || !defined(DEVICE_LPTICKER) || !defined(DEVICE_SLEEP)
#error [NOT_SUPPORTED] test not supported
#endif


#define HW_STATS_MAX_THREAD_STACK        384
#define HW_STATS_SAMPLE_TIME             5000    // msec





int wait_time = 1000;






void calc_cpu_usage()
{
    mbed_stats_cpu_t stats;
    mbed_stats_cpu_get(&stats);

    uint64_t diff = (stats.idle_time - prev_idle_time);
    uint8_t idle = (diff * 100) / (HW_STATS_SAMPLE_TIME*1000);    // usec;
    uint8_t usage = 100 - ((diff * 100) / (HW_STATS_SAMPLE_TIME*1000));    // usec;;
    prev_idle_time = stats.idle_time;

    APP_INFO(("Idle: %d Usage: %d \n", idle, usage));


}


void print_memory_info() {
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
    	APP_INFO(("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stats[i].max_size, stats[i].reserved_size));
    }
    free(stats);

    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    APP_INFO(("Heap size: %lu / %lu bytes\r\n", heap_stats.current_size, heap_stats.reserved_size));
}

void hw_stats(){
	calc_cpu_usage();
	print_memory_info();
}

void busy_thread()
{
    volatile uint64_t i = ~0;

    while(i--) {

    	wait_us(1000*wait_time);
    }
}


#endif











int main()
{

	    __enable_irq();



	    sound_init();

  while(true){





  }





}
