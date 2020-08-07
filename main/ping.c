#include <string.h>
#include <stdio.h>
#include "concord.h"

#define TOKEN "YOUR TOKEN"
#define CHANNEL 0

void
pong(concord_message_t *msg)
{

        if (!strcmp("ping", msg->content)) concord_reply("pong", msg);
}

void
app_main()
{
        concord_client_t *bot;
        puts(concord_discord_base_url);
        bot = NULL;
        bot = concord_new_client();
        concord_login(bot, TOKEN);
        bot->message_callback = pong;
        concord_get_channel_messages(CHANNEL, bot);
        concord_free_client(bot);
}
