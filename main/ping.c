#include <string.h>
#include <stdio.h>
#include "concord.h"


void
pong(concord_message_t *msg)
{

        if (!strcmp("ping", msg->content)) concord_reply("pong", msg);
}

void
app_main()
{
        concord_client_t *bot;

        bot = concord_new_client();
        concord_login(bot, CONFIG_CONCORD_TOKEN);
        bot->message_callback = pong;
        concord_get_channel_messages(atoll(CONFIG_CONCORD_CHANNEL_ID), bot);
        concord_free_client(bot);
}
