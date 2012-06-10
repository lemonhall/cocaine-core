//
// Copyright (C) 2011-2012 Rim Zaidullin <creator@bash.org.ru>
//
// Licensed under the BSD 2-Clause License (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <stdexcept>

#include <boost/current_function.hpp>

#include "cocaine/dealer/dealer.hpp"
#include "cocaine/dealer/response.hpp"
#include "cocaine/dealer/core/dealer_impl.hpp"

#include "cocaine/dealer/utils/error.hpp"

namespace cocaine {
namespace dealer {

dealer_t::dealer_t(const std::string& config_path) {
	impl_.reset(new dealer_impl_t(config_path));
}

dealer_t::~dealer_t() {
	impl_.reset();
}

boost::shared_ptr<response>
dealer_t::send_message(const void* data,
					 size_t size,
					 const message_path& path,
					 const message_policy& policy)
{
	boost::mutex::scoped_lock lock(mutex_);

	boost::shared_ptr<message_iface> msg = get_impl()->create_message(data, size, path, policy);
	boost::shared_ptr<response> resp(new response(get_impl(), msg->uuid(), path));
	get_impl()->send_message(msg, resp);

	return resp;
}

inline boost::shared_ptr<dealer_impl_t>
dealer_t::get_impl() {
	assert(impl_);
	return impl_;
}

} // namespace dealer
} // namespace cocaine
