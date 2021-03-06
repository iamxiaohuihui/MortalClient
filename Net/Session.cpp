//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#include "Session.h"

#include "../Configuration.h"

namespace jrc
{
Session::Session() noexcept : length(0), pos(0), connected(false)
{
}

Session::~Session() noexcept
{
    if (connected) {
        socket.close();
    }
}

bool Session::init(const char* host, const char* port)
{
    // Connect to the server.
    connected = socket.open(host, port);

    if (connected) {
        // Read keys neccessary for communicating with the server.
        cryptography = {socket.get_buffer()};
    }

    return connected;
}

Error Session::init()
{
    const std::string& host = Configuration::get().network.ip;
    if (host.empty()) {
        Console::get().print("No host IP was found in the settings file.");
        return Error::CONNECTION;
    }

    static std::string port
        = std::to_string(Configuration::get().network.port);

    if (!init(host.data(), port.data())) {
        return Error::CONNECTION;
    }

    return Error::NONE;
}

void Session::reconnect(const char* address, const char* port)
{
    // Close the current connection and open a new one.
    bool success = socket.close();

    if (success) {
        init(address, port);
    } else {
        connected = false;
    }
}

void Session::process(const std::int8_t* bytes, std::size_t available)
{
    if (pos == 0) {
        // Pos is 0, meaning this is the start of a new packet.
        // Start by determining length.
        length = cryptography.check_length(bytes);
        // Reading the length means we processed the header.
        // Move forward by the header length.
        bytes = bytes + HEADER_LENGTH;
        available -= HEADER_LENGTH;
    }

    // Determine how much we can write. Write data into the buffer.
    std::size_t towrite = length - pos;

    if (towrite > available) {
        towrite = available;
    }

    memcpy(buffer + pos, bytes, towrite);
    pos += towrite;

    // Check if the current packet has been fully processed.
    if (pos >= length) {
        cryptography.decrypt(buffer, length);

        try {
            packet_switch.forward(buffer, length);
        } catch (const PacketError& err) {
            Console::get().print(err.what());
        }

        pos = 0;
        length = 0;

        // Check if there is more available.
        const std::size_t remaining = available - towrite;

        if (remaining >= MIN_PACKET_LENGTH) {
            // More packets are available, so we start over.
            process(bytes + towrite, remaining);
        }
    }
}

bool Session::write(std::int8_t* packet_bytes,
                    std::size_t packet_length) noexcept
{
    if (!connected) {
        return false;
    }

    std::int8_t header[HEADER_LENGTH];
    cryptography.create_header(header, packet_length);
    cryptography.encrypt(packet_bytes, packet_length);

    if (!socket.dispatch(header, HEADER_LENGTH)) {
        return false;
    }
    return socket.dispatch(packet_bytes, packet_length);
}

void Session::read()
{
    // Check if a packet has arrived. Handle if data is sufficient:
    //     4 bytes(header) + 2 bytes(opcode) = 6.
    std::size_t result = socket.receive(&connected);

    if (result >= MIN_PACKET_LENGTH || length > 0) {
        // Retrieve buffer from the socket and process it.
        const std::int8_t* bytes = socket.get_buffer();
        process(bytes, result);
    }
}

bool Session::is_connected() const noexcept
{
    return connected;
}
} // namespace jrc
