typedef struct {
		char *content;
} concord_message;

typedef struct {
		void (*message_callback)(concord_message *);
		void (*ready_callback)();
		char *login;
} concord_client;

static void concord_reply(char *content, concord_message *msg);
static void concord_login(concord_client *client, char *token);
static void concord_free_client(concord_client *client);
static concord_client *concord_new_client();
