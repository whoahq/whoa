#ifndef NET_GRUNT_GRUNT_HPP
#define NET_GRUNT_GRUNT_HPP

namespace Grunt {
    class ClientLink;
    class ClientResponse;
    template <class>
    class Command;
    class Pending;
    class Timer;

    extern Command<ClientLink> s_clientCommands[];
}

#endif
