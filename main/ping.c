#include <string.h>
#include <stdio.h>
#include "concord.h"

#define TOKEN ""

void
pong(concord_message *msg)
{

        if (!strcmp("ping", msg->content)) concord_reply("pong", msg);
}

void
app_main()
{
        concord_client *bot;
        puts(concord_discord_base_url);
        bot = NULL;
        bot = concord_new_client();
        concord_login(bot, TOKEN);
        bot->message_callback = pong;
        concord_free_client(bot);
}
