package org.alexdev.kepler.messages.outgoing.messenger;

import org.alexdev.kepler.messages.types.MessageComposer;
import org.alexdev.kepler.server.netty.streams.NettyResponse;

public class FOLLOW_REQUEST extends MessageComposer {
    private final boolean isPublic;
    private final int roomId;

    public FOLLOW_REQUEST(boolean isPublic, int roomId) {
        this.isPublic = isPublic;
        this.roomId = roomId;
    }


    @Override
    public void compose(NettyResponse response) {
        response.writeBool(this.isPublic);
        response.writeInt(this.roomId);
    }

    @Override
    public short getHeader() {
        return 286; // "D^"
    }
}
