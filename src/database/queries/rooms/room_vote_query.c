#include "sqlite3.h"

#include "room_vote_query.h"
#include "database/db_connection.h"

#include "shared.h"

/**
 * Gets the user ID for whether they voted for that room or not.
 *
 * @param room_id the room id to check for
 * @param player_id the player id to check for
 * @return the user id, if successful, -1 if unsuccessful
 */
int room_query_check_voted(int room_id, int player_id) {
    // SELECT user_id FROM guestroom_votes WHERE user_id = ? AND room_id = ? LIMIT 1
    sqlite3 *conn = global.DB;
    sqlite3_stmt *stmt;

    int VOTED = -1;
    int status = sqlite3_prepare_v2(conn, "SELECT user_id FROM guestroom_votes WHERE user_id = ? AND room_id = ? LIMIT 1", -1, &stmt, 0);

    if (status == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, player_id);
        sqlite3_bind_int(stmt, 2, room_id);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(conn));
    }

    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW) {
        VOTED = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    //sqlite3_close(conn);

    return VOTED;
}

/**
 * Vote on a player room.
 *
 * @param room_id the room id to vote for
 * @param player_id the player id that voted
 * @param answer the answer that was given (thumbs up or thumbs down)
 */
void room_query_vote(int room_id, int player_id, int answer) {
    // INSERT INTO guestroom_votes (user_id,room_id,vote) VALUES (?,?,?)
    sqlite3 *conn = global.DB;
    sqlite3_stmt *stmt;

    int status = sqlite3_prepare_v2(conn, "INSERT INTO user_room_votes (user_id,room_id,vote) VALUES (?,?,?)", -1, &stmt, 0);

    if (status == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, player_id);
        sqlite3_bind_int(stmt, 2, room_id);
        sqlite3_bind_int(stmt, 3, answer);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(conn));
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("\nCould not step (execute) stmt. %s\n", sqlite3_errmsg(conn));
    }

    sqlite3_finalize(stmt);
    //sqlite3_close(conn);
}

/**
 * Get vote count for the room.
 *
 * @param room_id the room id to get the counts for
 * @return the total count
 */
int room_query_count_votes(int room_id) {
    // SELECT sum(vote) FROM guestroom_votes WHERE room_id = ? LIMIT 1
    sqlite3 *conn = global.DB;
    sqlite3_stmt *stmt;

    int VOTE_COUNT = -1;
    int status = sqlite3_prepare_v2(conn, " SELECT sum(vote) FROM user_room_votes WHERE room_id = ? LIMIT 1", -1, &stmt, 0);

    if (status == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(conn));
    }

    int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW) {
        VOTE_COUNT = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    //sqlite3_close(conn);

    return VOTE_COUNT;
}