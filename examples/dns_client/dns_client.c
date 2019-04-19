#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "re.h"


#define DEBUG_MODULE "dns_client"
#define DEBUG_LEVEL 7
#include <re_dbg.h>

struct srv_t {
    uint32_t srvc;
    struct sa srvv[16];
};

struct dns_query_t {
    struct dns_query *dq;
    char * host;
    uint32_t srvc;
    struct sa srvv[16];
};

static void signal_handler(int signum)
{
    (void)signum;

    re_fprintf(stderr, "terminated on signal %d\n", signum);

    re_cancel();
}

static bool rr_handler(struct dnsrr *rr, void *arg)
{
    struct dns_query_t *req = arg;

    if (req->srvc >= ARRAY_SIZE(req->srvv))
        return true;

    switch (rr->type) {

    case DNS_TYPE_A:
        sa_set_in(&req->srvv[req->srvc++], rr->rdata.a.addr,
              0);
        break;

    case DNS_TYPE_AAAA:
        sa_set_in6(&req->srvv[req->srvc++], rr->rdata.aaaa.addr,
               0);
        break;
    }

    return false;
}


static void query_handler(int err, const struct dnshdr *hdr, struct list *ansl,
              struct list *authl, struct list *addl, void *arg)
{
    struct dns_query_t *req = arg;
    (void)hdr;
    (void)authl;
    (void)addl;

    dns_rrlist_apply2(ansl, req->host, DNS_TYPE_A, DNS_TYPE_AAAA,
              DNS_CLASS_IN, true, rr_handler, req);
    if (req->srvc == 0) {
        err = err ? err : EDESTADDRREQ;
        goto fail;
    }

    /* print result */
    int i;
    re_fprintf(stdout, "Host %s :\n", req->host);
    for(i = 0; i< req->srvc; i++) {
        re_fprintf(stdout, "%J\n", &req->srvv[i]);
    }

    return;

 fail:
    return;
}


int main(int argc, const char * argv[])
{
    int err = 0;
    struct srv_t ns;
    struct dns_query_t *req = NULL;
    struct dnsc *dnsc = NULL;

    char host[] = "www.debian.org";

    req = mem_zalloc(sizeof(struct dns_query_t), NULL);
    if (!req)
        return ENOMEM;
    req->host = host;

    err = libre_init();
    if (err) {
        (void)re_fprintf(stderr, "libre_init: %m\n", err);
        goto out;
    }

    (void)sys_coredump_set(true);

    ns.srvc = ARRAY_SIZE(ns.srvv);

    /* fetch list of DNS server IP addresses */
    err = dns_srv_get(NULL, 0, ns.srvv, &ns.srvc);
    if (err) {
        re_fprintf(stderr, "unable to get dns servers: %s\n",
               strerror(err));
        goto out;
    } else {
        int i;
        re_fprintf(stdout, "Local NS:\n");
        for(i = 0; i< ns.srvc; i++) {
            re_fprintf(stdout, "%J\n", &ns.srvv[i]);
        }
    }

    /* create DNS client */
    err = dnsc_alloc(&dnsc, NULL, ns.srvv, ns.srvc);
    if (err) {
        re_fprintf(stderr, "unable to create dns client: %s\n",
               strerror(err));
        goto out;
    }

    /* act query process */
    err = dnsc_query(&req->dq, dnsc, host,
             DNS_TYPE_A, DNS_CLASS_IN, true,
             query_handler, req);
    if (err)
        goto out;

    /* main loop */
    (void)re_main(signal_handler);

    re_printf("Bye for now\n");

out:
    /* clean up/free all state */
    mem_deref(dnsc);
    mem_deref(req);

    /* free libre state */
    libre_close();

    /* Check for memory leaks */
    tmr_debug();
    mem_debug();

    return err;
}

