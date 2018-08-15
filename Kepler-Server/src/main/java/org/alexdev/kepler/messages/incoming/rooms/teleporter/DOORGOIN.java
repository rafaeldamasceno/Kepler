package org.alexdev.kepler.messages.incoming.rooms.teleporter;

import org.alexdev.kepler.dao.mysql.ItemDao;
import org.alexdev.kepler.game.item.Item;
import org.alexdev.kepler.game.item.base.ItemBehaviour;
import org.alexdev.kepler.game.player.Player;
import org.alexdev.kepler.game.room.Room;
import org.alexdev.kepler.game.room.RoomManager;
import org.alexdev.kepler.messages.outgoing.rooms.items.BROADCAST_TELEPORTER;
import org.alexdev.kepler.messages.types.MessageEvent;
import org.alexdev.kepler.server.netty.streams.NettyRequest;

public class DOORGOIN implements MessageEvent {
    @Override
    public void handle(Player player, NettyRequest reader) {
        Room room = player.getRoomUser().getRoom();

        if (room == null) {
            return;
        }

        int itemId = Integer.parseInt(reader.contents());
        Item item = room.getItemManager().getById(itemId);

        if (item == null || !item.hasBehaviour(ItemBehaviour.TELEPORTER)) {
            return;
        }

        if (!item.getPosition().equals(player.getRoomUser().getPosition())) {
            return;
        }

        Item linkedTeleporter = ItemDao.getItem(item.getTeleporterId());

        if (linkedTeleporter == null) {
            return;
        }

        if (RoomManager.getInstance().getRoomById(linkedTeleporter.getRoomId()) == null) {
            return;
        }

        room.send(new BROADCAST_TELEPORTER(item, player.getDetails().getName(), true));
    }
}