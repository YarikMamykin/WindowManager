#pragma once

namespace ymwm::errors {
	struct Handler {
		// Implementation defined for each environment individually.
		template<typename... Args>
		void operator()(Args... args) const;
	};
}
