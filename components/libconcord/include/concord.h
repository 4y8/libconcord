#ifndef __CONCORD_H__
#define __CONCORD_H__
typedef struct {
		char *content;
		int id;
		int channel_id;
} concord_message;

typedef struct {
		void (*message_callback)(concord_message *);
		void (*ready_callback)();
		char *login;
} concord_client;

char *concord_discord_base_url = "https://discord.com/api/v6";

static void concord_reply(char *content, concord_message *msg);
static void concord_login(concord_client *client, char *token);
static void concord_free_client(concord_client *client);
static concord_message *concord_get_channel_messages(int channel_id);
static concord_client *concord_new_client();
#endif /* concord.h */
