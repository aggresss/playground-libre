#include <stdio.h>
#include <assert.h>
#include "re.h"



int main(int argc, const char * argv[])
{
    // setup
    struct mbuf * test_mbuf = NULL;
    test_mbuf = mbuf_alloc(256);

    // assertion
    uint8_t * get_buf = mbuf_buf(test_mbuf);
    assert(get_buf == test_mbuf->buf);

    // teardown
    mem_deref(test_mbuf);

    mem_debug();
    return 0;
}

