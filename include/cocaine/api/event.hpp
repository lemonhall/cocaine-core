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

#ifndef COCAINE_EVENT_API_HPP
#define COCAINE_EVENT_API_HPP

#include "cocaine/common.hpp"

namespace cocaine { namespace api {

struct policy_t {
    policy_t():
        urgent(false),
        timeout(0.0f),
        deadline(0.0f)
    { }

    policy_t(bool urgent_,
             double timeout_,
             double deadline_):
        urgent(urgent_),
        timeout(timeout_),
        deadline(deadline_)
    { }

    bool urgent;
    double timeout;
    double deadline;
};

struct event_t {
    event_t(const std::string& type_):
        type(type_)
    { }

    event_t(const std::string& type_,
            policy_t policy_):
        type(type_),
        policy(policy_)
    { }

public:
    // Event type.
    const std::string type;

    // Event execution policy.
    const policy_t policy;
};

}} // namespace cocaine::api

#endif
