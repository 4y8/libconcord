#ifndef __CONCORD_H__
#define __CONCORD_H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "esp_tls.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

static void
_concord_get_channel_messages(void *pvParameter)
{
		long *p;
		long long channel_id;
		concord_client_t *client;
		concord_message_t *message_buf;
		char request[512], url[256], ret_buf[512];
		int ret, len;

		p = (long *)pvParameter;
		printf("%ld\n", (long)*p);
		vTaskDelete(0);
		channel_id = (long long)*p;
		client = (concord_client_t *)*(p + 1);
		sprintf(url, "https://www.discord.com/api/v6/channels/%lld/messages",
				channel_id);
		sprintf(request,
				"GET %s HTTP/1.0\r\n"
				"Host: discord.com\r\n"
				"User-Agent: esp-idf/1.0 esp32\r\n"
				"Authorization: Bot %s\r\n"
				"\r\n",
				url, client->login);
		char *tag = "ESP";
		ESP_LOGI(tag, "%s, %s", url, client->login);
		esp_tls_cfg_t cfg = {
				.crt_bundle_attach = esp_crt_bundle_attach,
		};
		esp_tls_t *tls = esp_tls_conn_http_new(url, &cfg);
		if(!tls) return;
		size_t written_bytes = 0;

		do {
				ret = esp_tls_conn_write(tls,
										 request + written_bytes,
										 strlen(request) - written_bytes);
				if (ret >= 0) written_bytes += ret;
				else if (ret != ESP_TLS_ERR_SSL_WANT_READ  &&
						   ret != ESP_TLS_ERR_SSL_WANT_WRITE)
						return;
		} while (written_bytes < strlen(request));
		do {
				len = sizeof(ret_buf) - 1;
				bzero(ret_buf, sizeof(ret_buf));
				ret = esp_tls_conn_read(tls, (char *)ret_buf, len);

				if(ret == ESP_TLS_ERR_SSL_WANT_WRITE  ||
				   ret == ESP_TLS_ERR_SSL_WANT_READ)
						continue;

				if(ret <= 0) break;

				len = ret;
				/* Print response directly to stdout as it is read */
				for(int i = 0; i < len; i++)
						putchar(ret_buf[i]);
		} while(1);
		printf("\n");
		esp_tls_conn_delete(tls);
		vTaskDelete(0);
		return;
}

static concord_message_t *
concord_get_channel_messages(long long channel_id, concord_client_t *client)
{
		long *p;
		p = malloc(sizeof(long) * 3);
		*p = 1;// (long)&channel_id;
		// *(p + 1) = (long)client;
		printf("%ld\n", (long)*p);
		// *(p + 2) = 0;
		xTaskCreate(&_concord_get_channel_messages, "main", 8192, p, 5, NULL);
		free(p);
		return NULL;
}
#endif /* concord.h */
