#ifndef __CONCORD_H__
#define __CONCORD_H__
#include <stdlib.h>
#include <stdio.h>
#include "esp_tls.h"
typedef struct {
		char *content;
		int id;
		int channel_id;
} concord_message_t;

typedef struct {
		void (*message_callback)(concord_message_t *);
		void (*ready_callback)();
		char *login;
} concord_client_t;

char *concord_discord_base_url = "https://discord.com";

static void concord_reply(char *, concord_message_t *);
static void concord_login(concord_client_t *, char *);
static void concord_free_client(concord_client_t *);
static concord_message_t *concord_get_channel_messages(int, concord_client_t *);
static concord_client_t *concord_new_client(void);

static void
concord_reply(char *content, concord_message_t *msg)
{

}

static void
concord_login(concord_client_t *client, char *token)
{

}

static void
concord_free_client(concord_client_t *client)
{

	free(client);
}

static concord_client_t *
concord_new_client()
{

	return NULL;
}

static concord_message_t *
concord_get_channel_messages(int channel_id, concord_client_t *client)
{
	char request[512];

	esp_tls_cfg_t cfg = {
		.crt_bundle_attach = esp_crt_bundle_attach,
	};
	struct esp_tls *tls = esp_tls_conn_http_new(WEB_URL, &cfg);
	sprintf(request,
		"GET https://discord.com/apt/v6/channels/%d/messages HTTP/1.0\r\n"
		"Host: www.discord.com\r\n"
		"User-Agent: esp-idf/1.0 esp32\r\n"
		"Authorization: Bot %s\r\n"
		"\r\n",
		channel_id, client->login);
	return NULL;
}

#endif /* concord.h */
