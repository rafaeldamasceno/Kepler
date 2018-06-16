package org.alexdev.kepler.game.commands.registered;

import org.alexdev.kepler.dao.mysql.ItemDao;
import org.alexdev.kepler.game.commands.Command;
import org.alexdev.kepler.game.entity.Entity;
import org.alexdev.kepler.game.entity.EntityType;
import org.alexdev.kepler.game.item.Item;
import org.alexdev.kepler.game.item.base.ItemBehaviour;
import org.alexdev.kepler.game.player.Player;
import org.alexdev.kepler.messages.outgoing.rooms.user.CHAT_MESSAGE;

import java.util.ArrayList;
import java.util.List;

public class PickupCommand extends Command {
    @Override
    public void addPermissions() {
        this.permissions.add("default");
    }

    @Override
    public void handleCommand(Entity entity, String message, String[] args) {
        if (entity.getType() != EntityType.PLAYER) {
            return;
        }

        Player player = (Player) entity;

        if (player.getRoom() == null) {
            System.out.println("lol 123");
            return;
        }

        if (!player.getRoom().isOwner(player.getEntityId())) {
            System.out.println("lol 123 4");
            return;
        }

        List<Item> itemsToUpdate = new ArrayList<>();

        for (Item item : player.getRoom().getItems()) {
            if (item.hasBehaviour(ItemBehaviour.PUBLIC_SPACE_OBJECT)) {
                continue; // Cannot pick up public room furniture.
            }

            itemsToUpdate.add(item);
        }

        for (Item item : itemsToUpdate) {
            item.setOwnerId(player.getEntityId());

            player.getInventory().getItems().add(item);
            player.getRoom().getMapping().removeItem(item);
        }

        ItemDao.updateItems(itemsToUpdate);

        player.getInventory().getView("last");
        player.send(new CHAT_MESSAGE(CHAT_MESSAGE.type.WHISPER, player.getRoomUser().getInstanceId(), "All furniture items have been picked up"));
    }

    @Override
    public String getDescription() {
        return "Allows the owner to pick up all furniture in a room";
    }
}
