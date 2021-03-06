package org.alexdev.kepler.messages.incoming.messenger;

import org.alexdev.kepler.dao.mysql.MessengerDao;
import org.alexdev.kepler.game.messenger.MessengerUser;
import org.alexdev.kepler.game.player.Player;
import org.alexdev.kepler.game.player.PlayerManager;
import org.alexdev.kepler.messages.types.MessageEvent;
import org.alexdev.kepler.server.netty.streams.NettyRequest;

import java.sql.SQLException;

public class MESSENGER_DECLINEBUDDY implements MessageEvent {
    @Override
    public void handle(Player player, NettyRequest reader) throws SQLException {
        reader.readInt(); // Junk wtf

        int amount = reader.readInt();

        for (int i = 0; i < amount; i++) {
            int userId = reader.readInt();

            if (!player.getMessenger().hasRequest(userId)) {
                continue;
            }

            MessengerDao.removeRequest(userId, player.getDetails().getId());
            MessengerDao.removeRequest(player.getDetails().getId(), userId);

            player.getMessenger().getRequests().remove(player.getMessenger().getRequest(userId));

            Player friend = PlayerManager.getInstance().getPlayerById(userId);

            if (friend != null) {
                friend.getMessenger().getRequests().remove(friend.getMessenger().getRequest(player.getDetails().getId()));
            }
        }
    }
}
