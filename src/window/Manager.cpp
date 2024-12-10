#include "Manager.h"

#include <format>
#include <iostream>

namespace ymwm::window {

  Manager::Manager() { m_windows.reserve(5); }

  void Manager::add_window(const Window& w) noexcept {
    std::cout << std::format("{} {} {} {} {}\n", w.id, w.x, w.y, w.h, w.w);
    m_windows.push_back(w);
  }

  void Manager::remove_window(environment::ID id) noexcept {
    auto erased_successfully = std::erase_if(
        m_windows, [id](const Window& w) -> bool { return id == w.id; });
    if (erased_successfully) {
      std::cout << std::format("Erased {} \n", id);
    }
  }

  const std::vector<Window>& Manager::windows() const noexcept {
    return m_windows;
  }

  bool Manager::has_window(environment::ID id) const noexcept {
    return m_windows.end() !=
           std::find_if(m_windows.begin(),
                        m_windows.end(),
                        [id](const auto& w) -> bool { return id == w.id; });
  }
} // namespace ymwm::window
