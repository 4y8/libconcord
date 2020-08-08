#ifndef __CONCORD_H__
#define __CONCORD_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_tls.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"

typedef struct {
	char *name;
} concord_user_t;

typedef struct {
	char *content;
	int   id;
	int   channel_id;
} concord_message_t;

typedef struct {
	concord_message_t *messages;
	int                length;
} concord_message_list_t;

typedef struct {
	void (*message_callback)(concord_message_t *);
	void (*ready_callback)();
	char *login;
} concord_client_t;

static void concord_reply(char *, concord_message_t *);
static void concord_login(concord_client_t *, char *);
static void concord_free_client(concord_client_t *);
static concord_message_t *concord_get_channel_messages(long long, concord_client_t *);
static concord_client_t *concord_new_client(void);

static void
concord_reply(char *content, concord_message_t *msg)
{

}

static void
concord_login(concord_client_t *client, char *token)
{

	client->login = strdup(token);
}

static void
concord_free_client(concord_client_t *client)
{

	free(client->login);
	free(client);
}

static concord_client_t *
concord_new_client()
{
	concord_client_t *client;

	client = malloc(sizeof(concord_client_t));
	if (!client) printf("Unable to allocate memory");
	return client;
}

static int
read_char_tls(esp_tls_t *tls)
{
	char c;
	int ret;
	
	do {
		ret = esp_tls_conn_read(tls, &c, 1);
		if (ret <= 0) return -1;
	} while (ret == ESP_TLS_ERR_SSL_WANT_WRITE ||
		 ret == ESP_TLS_ERR_SSL_WANT_READ);
	return c;
}

static concord_message_list_t *
concord_get_channel_messages(long long channel_id, concord_client_t *client)
{
	char request[512], url[256];
	int ret, loop, nbracket, ncomma, c;

	sprintf(url, "https://www.discord.com/api/v6/channels/%lld/messages",
		channel_id);
	sprintf(request,
		"GET %s HTTP/1.0\r\n"
		"Host: discord.com\r\n"
		"User-Agent: esp-idf/1.0 esp32\r\n"
		"Authorization: Bot %s\r\n"
		"\r\n",
		url, client->login);
	esp_tls_cfg_t cfg = {
		.crt_bundle_attach = esp_crt_bundle_attach,
	};
	esp_tls_t *tls = esp_tls_conn_http_new(url, &cfg);
	if (!tls) return NULL;
	size_t written_bytes = 0;

	/* Send the request. */
	do {
		ret = esp_tls_conn_write(tls, request + written_bytes,
					 strlen(request) - written_bytes);
		if (ret >= 0) written_bytes += ret;
		else if (ret != ESP_TLS_ERR_SSL_WANT_READ &&
			 ret != ESP_TLS_ERR_SSL_WANT_WRITE)
			return NULL;
	} while (written_bytes < strlen(request));

	loop = 0;
	/* Skip the answer's header. */
	while (loop != 4) {
		c = read_char_tls(tls);
		if      (c == '\r') ++loop;
		else if (c == '\n') ++loop;
		else              loop = 0;
	} /* Receive the answer */
	nbracket = 0;
	ncomma   = 0;
	esp_tls_conn_read(tls, &c, 1);
	c = 0;
	while (c != -1) {
		c = read_char_tls(tls);
		if (c == ',')      ++ncomma;
		else if (c == '{') ++nbracket;
		else if (c == '}') --nbracket;
		if (!nbracket) ncomma = 0;
		else if (ncomma == 2 || ncomma == 5) {
			while (read_char_tls(tls) != ':');
			while (read_char_tls(tls) == ' ');
			while ((c = read_char_tls(tls)) != '"') putchar(c);
			printf("\n");
		}
	} printf("\n");
	esp_tls_conn_delete(tls);
	return NULL;
}

#endif /* concord.h */
