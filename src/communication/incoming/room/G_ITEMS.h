#include "communication/messages/incoming_message.h"
#include "communication/messages/outgoing_message.h"

void G_ITEMS(player *player, incoming_message *message) {
    outgoing_message *om = om_create(45); // "@m"
    player_send(player, om);
    om_cleanup(om);
}