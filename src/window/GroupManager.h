#pragma once
#include "Manager.h"

#include <list>

namespace ymwm::window {

  template <typename Environment>
  struct GroupManager {
    inline GroupManager(Environment* env)
        : m_env(env)
        , m_active_manager(0ul) {
      m_managers.emplace_back(m_env);
    }

    inline Manager<Environment>& manager() noexcept {
      return *std::next(m_managers.begin(), m_active_manager);
    }

    inline void add() noexcept {
      m_managers.emplace_back(m_env);
      next();
    }

    inline void next() noexcept {
      if (one_manager_present()) {
        return;
      }

      manager().deactivate();

      m_active_manager = is_last_active() ? 0ul : m_active_manager + 1ul;

      manager().activate();
    }

    inline void prev() noexcept {
      if (one_manager_present()) {
        return;
      }

      manager().deactivate();

      m_active_manager =
          is_first_active() ? m_managers.size() - 1ul : m_active_manager - 1ul;

      manager().activate();
    }

    inline void activate(std::size_t manager_index) noexcept {
      if (not valid_index(manager_index)) {
        return;
      }

      manager().deactivate();
      m_active_manager = manager_index;
      manager().activate();
    }

    inline ~GroupManager() = default;

  private:
    inline bool valid_index(std::size_t manager_index) noexcept {
      return manager_index < m_managers.size();
    }

    inline bool one_manager_present() const noexcept {
      return 1ul == m_managers.size();
    }

    inline bool is_first_active() const noexcept {
      return 0ul == m_active_manager;
    }

    inline bool is_last_active() const noexcept {
      return (m_managers.size() - 1ul) == m_active_manager;
    }

    Environment* const m_env;
    std::size_t m_active_manager;
    std::list<Manager<Environment>> m_managers;
  };
} // namespace ymwm::window
