#pragma once
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/endian/conversion.hpp>
#include <sstream>
#include <string>
#include "../protocol/data_protocol.h"

#include <iostream>

class Communicator {
public:
    void SerializeAndSendPacket(const ProtocolPacket& packet, boost::asio::ip::tcp::socket& socket) {
        // Step 1: Serialize ProtocolPacket to string
        std::ostringstream oss(std::ios_base::binary);
        boost::archive::binary_oarchive oa(oss);
        oa << packet;
        std::string serializedDataStr = oss.str();

        // Step 2: Get the size of data and send it
        std::uint64_t dataSize = serializedDataStr.size();
        boost::endian::native_to_big_inplace(dataSize);
        boost::asio::write(socket, boost::asio::buffer(&dataSize, sizeof(dataSize)));

        // Step 3: Send data through socket
        boost::asio::write(socket, boost::asio::buffer(serializedDataStr));
    }

    void ReceiveAndDeserializePacket(ProtocolPacket& packet, boost::asio::ip::tcp::socket& socket) {
        // Step 1: Receive the size of the data
        std::uint64_t dataSize = 0;
        while (socket.available() < sizeof(dataSize)) {
            // Wait for input
        }
        boost::asio::read(socket, boost::asio::buffer(&dataSize, sizeof(dataSize)));
        boost::endian::big_to_native_inplace(dataSize);

        // Step 2: Receive data from socket
        std::vector<char> buf(dataSize);
        std::size_t bytesRead = 0;
        while (bytesRead < dataSize)
        {
            if (socket.available() > 0) {
                bytesRead += socket.read_some(boost::asio::buffer(buf.data() + bytesRead, dataSize - bytesRead));
            }
        }

        // Step 3: Save data to a string
        std::string receivedDataStr(buf.begin(), buf.end());

        // Step 4: Deserialize data to ProtocolPacket
        std::istringstream iss(receivedDataStr, std::ios_base::binary);
        boost::archive::binary_iarchive ia(iss);
        ia >> packet;
    }
};