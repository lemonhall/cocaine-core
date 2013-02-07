/*
    Copyright (c) 2011-2012 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2012 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COCAINE_ENGINE_HPP
#define COCAINE_ENGINE_HPP

#include "cocaine/common.hpp"
#include "cocaine/asio.hpp"
#include "cocaine/atomic.hpp"

#include "cocaine/api/isolate.hpp"

#include <deque>
#include <set>

#include <boost/thread/mutex.hpp>

namespace cocaine { namespace engine {

class session_queue_t:
    public std::deque<boost::shared_ptr<session_t>>
{
    public:
        void
        push(const_reference session);

        // Lockable concept implementation

        void
        lock() {
            m_mutex.lock();
        }

        void
        unlock() {
            m_mutex.unlock();
        }

    private:
        boost::mutex m_mutex;
};

class engine_t:
    public boost::noncopyable
{
    enum class state_t: int {
        running,
        broken,
        stopping,
        stopped
    };

    public:
        engine_t(context_t& context,
                 const manifest_t& manifest,
                 const profile_t& profile);

        ~engine_t();

        void
        run();

        // Scheduling

        boost::shared_ptr<api::stream_t>
        enqueue(const api::event_t& event,
                const boost::shared_ptr<api::stream_t>& upstream);

        // Slave I/O

        void
        tie(const boost::shared_ptr<io::codex<io::pipe_t>>& codex,
            const unique_id_t& uuid);

        void
        wake();

    public:
        ev::loop_ref&
        loop() {
            return m_loop;
        }

    private:
        void
        on_connection(ev::io&, int);

        void
        on_ctl_event(ev::io&, int);

        void
        on_ctl_check(ev::prepare&, int);

        void
        on_cleanup(ev::timer&, int);

        void
        on_notification(ev::async&, int);

        void
        on_termination(ev::timer&, int);

        void
        process_ctl_events();

        void
        pump();

        void
        balance();

        void
        migrate(state_t target);

        void
        stop();

    private:
        context_t& m_context;
        std::unique_ptr<logging::log_t> m_log;

        const manifest_t& m_manifest;
        const profile_t& m_profile;

        // Engine state

        state_t m_state;

        // I/O

        std::unique_ptr<io::acceptor_t> m_acceptor;
        std::unique_ptr<io::socket_t> m_ctl;

        // Event loop

        ev::dynamic_loop m_loop;

        ev::io m_connection_watcher;

        ev::io m_ctl_watcher;
        ev::prepare m_ctl_checker;

        ev::timer m_gc_timer,
                  m_termination_timer;

        ev::async m_notification;

        // Auto-incrementing Session ID

        std::atomic<uint64_t> m_next_id;

        // Session queue

        session_queue_t m_queue;

        // Slave pool

        std::set<handshake_t*> m_handshakes;

#if BOOST_VERSION >= 103600
        typedef boost::unordered_map<
#else
        typedef std::map<
#endif
            unique_id_t,
            boost::shared_ptr<slave_t>
        > pool_map_t;

        pool_map_t m_pool;

        // NOTE: A strong isolate reference, keeping it here
        // avoids isolate destruction, as the factory stores
        // only weak references to the isolate instances.
        api::category_traits<api::isolate_t>::ptr_type m_isolate;
};

}} // namespace cocaine::engine

#endif
