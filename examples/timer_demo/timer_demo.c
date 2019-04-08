#include <stdio.h>
#include <assert.h>
#include "re.h"
#include "inttypes.h"

#define DEBUG_MODULE "timer_demo"
#define DEBUG_LEVEL 7
#include <re_dbg.h>

static void signal_handler(int signum)
{
    (void)signum;

    re_fprintf(stderr, "terminated on signal %d\n", signum);

    re_cancel();
}

static void timer_hander(void * args)
{
    re_fprintf(stdout, "timer test on %"PRIu64"\n", tmr_jiffies());
}

int main(int argc, const char * argv[])
{
    int err = 0;
    struct tmr test_timer;

    err = libre_init();
    if (err) {
        (void)re_fprintf(stderr, "libre_init: %m\n", err);
        goto out;
    }

    (void)sys_coredump_set(true);


    tmr_init(&test_timer);
    tmr_start(&test_timer, 10*1000, timer_hander, NULL);

    (void)re_main(signal_handler);

    re_printf("Bye for now\n");

out:
    libre_close();

    /* Check for memory leaks */
    tmr_debug();
    mem_debug();

    return err;
}

