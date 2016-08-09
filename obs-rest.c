#include <obs-module.h>
#include <util/threading.h>

#include "mongoose.h"

static const char *s_http_port = "8000";

pthread_t mg_thread;

static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;
    char reply[100];

    /* Simulate long calculation */
    sleep(3);

    /* Send the reply */
    snprintf(reply, sizeof(reply), "{ \"uri\": \"%.*s\" }\n", (int) hm->uri.len,
             hm->uri.p);
    mg_printf(c,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: application/json\r\n"
              "Content-Length: %d\r\n"
              "\r\n"
              "%s",
              (int) strlen(reply), reply);
  }
}

static void *server_thread(void *param)
{
	struct mg_mgr mgr;
	struct mg_connection *nc;

	mg_mgr_init(&mgr, NULL);
	nc = mg_bind(&mgr, s_http_port, ev_handler);
	mg_set_protocol_http_websocket(nc);

	/* For each new connection, execute ev_handler in a separate thread */
	mg_enable_multithreading(nc);

	printf("Starting multi-threaded server on port %s\n", s_http_port);

	for (;;) {
		mg_mgr_poll(&mgr, 3000);
	}
	mg_mgr_free(&mgr);

	return NULL;
}

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("remote", "en-US")

bool obs_module_load(void)
{
	pthread_create(&mg_thread, NULL, server_thread, NULL);

	return true;
}

