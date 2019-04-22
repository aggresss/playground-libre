#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "re.h"


#define DEBUG_MODULE "http_client"
#define DEBUG_LEVEL 7
#include <re_dbg.h>



static void signal_handler(int signum)
{
    (void)signum;

    re_fprintf(stderr, "terminated on signal %d\n", signum);

    re_cancel();
}

static void http_resp_handler(int err, const struct http_msg *msg, void *arg)
{
    FILE ** f = arg;
    bool chunked;

    if (err) {
        /* translate error code */
        err = ENOMEM;
        goto out;
    }

    if(*f) {
        fclose(*f);
    }
#if 1
    re_printf("%H\n", http_msg_print, msg);
    re_printf("BODY: %b\n", msg->mb->buf, msg->mb->end);
#endif

    chunked = http_msg_hdr_has_value(msg, HTTP_HDR_TRANSFER_ENCODING,
                     "chunked");

 out:
    return;
}

static int http_data_handler(const uint8_t *buf, size_t size,
                 const struct http_msg *msg, void *arg)
{
    FILE ** f = arg;
    (void)msg;
    int write_len;

    re_printf("receive data %zu\n", size);

    if(!*f)
    {
        *f = fopen("file.file", "w");
        if(!*f)
        {
            re_printf("fopen %s error: %m\n", *f);
            return -1;
        }
    }
    write_len = fwrite(buf, sizeof(char), size, *f);
    if(write_len < size && errno != EINTR) {
        re_printf("fwrite error: %m\n");
        return -1;
    }
    re_printf("fwrite : %d\n", write_len);
    return 0;
}

int main(int argc, const char * argv[])
{
    struct http_cli *cli = NULL;
    struct http_req *req = NULL;
    struct dnsc *dnsc = NULL;
    uint32_t nsc;
    struct sa nsv[16];
    char url[256];
    FILE * fptr = NULL;
    int err; /* errno return values */

    /* enable coredumps to aid debugging */
    (void)sys_coredump_set(true);

    /* initialize libre state */
    err = libre_init();
    if (err) {
        re_fprintf(stderr, "re init failed: %s\n", strerror(err));
        goto out;
    }

    nsc = ARRAY_SIZE(nsv);

    /* fetch list of DNS server IP addresses */
    err = dns_srv_get(NULL, 0, nsv, &nsc);
    if (err) {
        re_fprintf(stderr, "unable to get dns servers: %s\n",
               strerror(err));
        goto out;
    }

    /* create DNS client */
    err = dnsc_alloc(&dnsc, NULL, nsv, nsc);
    if (err) {
        re_fprintf(stderr, "unable to create dns client: %s\n",
               strerror(err));
        goto out;
    }

    /* create HTTP client */
    err = http_client_alloc(&cli, dnsc);
    if (err) {
        re_fprintf(stderr, "unable to create http client: %s\n",
               strerror(err));
        goto out;
    }

    (void)re_snprintf(url, sizeof(url), "http://link.router7.com:8091/index.html");

    err = http_request(&req, cli, "GET", url,
                http_resp_handler, http_data_handler, &fptr, NULL);
    if (err)
        goto out;

    /* main loop */
    (void)re_main(signal_handler);

    re_printf("Bye for now\n");

out:
    /* clean up/free all state */
    mem_deref(req);
    mem_deref(cli);
    mem_deref(dnsc);

    /* free libre state */
    libre_close();

    /* Check for memory leaks */
    tmr_debug();
    mem_debug();

    return err;
}

