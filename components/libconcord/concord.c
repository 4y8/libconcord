#include <stdlib.h>
#include "include/concord.h"

void
concord_free_client(concord_client *client)
{

    free(client);
}
