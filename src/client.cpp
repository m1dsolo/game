#include <game/client.hpp>

#include <thread>
#include <cstring>

#include <wheel/log.hpp>

namespace wheel {

void Client::connect() {
    Log::assert(socket_.init(), "socket init failed");
    Log::assert(socket_.connect("127.0.0.1", 12345), "socket connect failed");
    std::thread t([&]() {
        memset(buf_, 0, sizeof(buf_));  // TODO: need?
        while (socket_.recv(buf_)) {
            queue_.push(std::string(buf_));
        }
        Log::assert(false, "socket recv failed");
    });
    t.detach();

    send("group=1");
    usleep(10);

    connected_ = true;
}

bool Client::send(std::string_view msg) {
    if (connected_) {
        return socket_.send(msg);
    } else {
        queue_.push(msg.data());
        return true;
    }
}

}  // namespace wheel
