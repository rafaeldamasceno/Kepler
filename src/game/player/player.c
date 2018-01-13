#include <stdio.h>

#include "player.h"
#include "dyad.h"

#include "util/stringbuilder.h"
#include "communication/messages/outgoing_message.h"

#include "shared.h"

/**
 * Creates a new player
 * @return player struct
 */
player *player_create(dyad_Stream *stream) {
    player *player = malloc(sizeof(player));
    player->stream = stream;
    player->player_data = NULL;
    return player;
}

/**
 * Creates a new player data instance
 * @return player data struct
 */
player_data *player_create_data(int id, char *username, char *figure, int credits, char *motto, char *sex, int tickets, int film) {
    player_data *data = malloc(sizeof(player_data));
    data->id = id;
    data->username = strdup(username);
    data->figure = strdup(figure);
    data->credits = credits;
    data->motto = strdup(motto);
    data->sex = strdup(sex);
    data->tickets = tickets;
    data->film = film;
    return data;
}

/**
 * Called when a connection is successfully opened
 * @param p the player struct
 */
void player_login(player *player) {
    // Add rooms
    room_manager_add_by_user_id(player->player_data->id);
}

/**
 * Called when a connection is closed
 * @param player the player struct
 */
void player_cleanup(player *player) {
    if (player == NULL) {
        return;
    }

    player_data *player_data = player->player_data;

    if (player_data != NULL) {
        free(player_data->username);
        free(player_data->figure);
        free(player_data->motto);
        free(player_data->sex);
        free(player_data);
    }

    printf("Client [%s] has disconnected\n", dyad_getAddress(player->stream));
    free(player);
}

/**
 * Send an outgoing message to the socket
 * @param p the player struct
 * @param om the outgoing message
 */
void player_send(player *p, outgoing_message *om) {
    om_finalise(om);

    char *data = om->sb->data;
    dyad_write(p->stream, data, strlen(data));

    //printf ("Client [%s] outgoing data: %i / %s\n", dyad_getAddress(p->stream), om->header_id, data);
}

/**
 * Sends the key of an error, whose description value is inside the external_texts of the client.
 * @param p the player
 * @param error the error message
 */
void send_localised_error(player *p, char *error) {
    outgoing_message *om = om_create(33); // @a
    om_write_str(om, error);
    player_send(p, om);
    om_cleanup(om);
}

/**
 * Send an alert to the player
 * @param p the player
 * @param greeting the alert message
 */
void send_alert(player *p, char *greeting) {
    outgoing_message *welcome_message = om_create(139); // BK
    om_write_str(welcome_message, greeting);
    player_send(p, welcome_message);
    om_cleanup(welcome_message);
}