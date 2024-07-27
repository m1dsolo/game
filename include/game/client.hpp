#pragma once

#include <wheel/socket.hpp>
#include <wheel/safe_queue.hpp>
#include <wheel/singleton.hpp>

namespace wheel {

class Client : public Singleton<Client> {
    friend class Singleton<Client>;

public:
    void connect();

    bool send(std::string_view msg);

    SafeQueue<std::string>& get_queue() { return queue_; }
    const SafeQueue<std::string>& get_queue() const { return queue_; }

private:
    Client() = default;
    Client(const Client&) = delete;

    Socket socket_;
    bool connected_ = false;

    char buf_[1024];
    SafeQueue<std::string> queue_;
};

}  // namespace wheel
