/*
    Copyright (c) 2011-2013 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2013 Other contributors as noted in the AUTHORS file.

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

#ifndef COCAINE_APP_HPP
#define COCAINE_APP_HPP

#include "cocaine/common.hpp"
#include "cocaine/dynamic.hpp"

#include <thread>

namespace cocaine { namespace api {

struct event_t;
struct stream_t;

}} // namespace cocaine::api

namespace cocaine { namespace engine {

class engine_t;

struct manifest_t;
struct profile_t;

}} // namespace cocaine::engine

namespace cocaine {

class app_t {
    COCAINE_DECLARE_NONCOPYABLE(app_t)

    context_t& m_context;

    const std::unique_ptr<logging::log_t> m_log;

    // Configuration

    std::unique_ptr<const engine::manifest_t> m_manifest;
    std::unique_ptr<const engine::profile_t> m_profile;

    // Control

    std::unique_ptr<io::reactor_t> m_reactor;
    std::unique_ptr<io::channel<io::socket<io::local>>> m_engine_control;

    // Engine

    std::shared_ptr<engine::engine_t> m_engine;
    std::unique_ptr<std::thread> m_thread;

public:
    app_t(context_t& context, const std::string& name, const std::string& profile);
   ~app_t();

    void
    start();

    void
    stop();

    dynamic_t
    info() const;

    // Scheduling

    std::shared_ptr<api::stream_t>
    enqueue(const api::event_t& event, const std::shared_ptr<api::stream_t>& upstream);

    std::shared_ptr<api::stream_t>
    enqueue(const api::event_t& event, const std::shared_ptr<api::stream_t>& upstream, const std::string& tag);
};

} // namespace cocaine

#endif
