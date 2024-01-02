#pragma once

#include <gkr/api.hpp>
#include <gkr/concurency/events_waiting.hpp>

#include <vector>

namespace gkr
{
#ifdef _WIN32

class win_events_waiter : public events_waiter
{
    win_events_waiter           (const win_events_waiter&) noexcept = delete;
    win_events_waiter& operator=(const win_events_waiter&) noexcept = delete;

    win_events_waiter           (win_events_waiter&&) noexcept = delete;
    win_events_waiter& operator=(win_events_waiter&&) noexcept = delete;

public:
    GKR_CORE_API win_events_waiter(std::size_t flags = 0) noexcept;
    GKR_CORE_API virtual ~win_events_waiter() noexcept;

private:
    alignas(void*)
    mutable char       m_section[40] {};
    std::vector<void*> m_events      {};
    std::size_t        m_waiting_threads {0};

public:
    GKR_CORE_API virtual std::size_t events_count() const noexcept override;

    GKR_CORE_API virtual bool pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual bool remove_all_events() noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_CORE_API virtual bool add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false) override;

    GKR_CORE_API virtual bool fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual bool reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_CORE_API virtual wait_result_t check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual wait_result_t wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT) override;

public:
    GKR_CORE_API virtual wait_result_t check_all_events() noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual wait_result_t wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT) override;

private:
    bool this_thread_can_wait() const noexcept;

    wait_result_t check_events(std::size_t preset_index) const noexcept;

    wait_result_t wait_events(unsigned timeout) noexcept(DIAG_NOEXCEPT);

    wait_result_t check_event(std::size_t index) const noexcept;

    wait_result_t wait_event(std::size_t index, unsigned timeout) noexcept(DIAG_NOEXCEPT);
};

using os_events_waiter = win_events_waiter;

#else

class linux_events_waiter : public events_waiter
{
    linux_events_waiter           (const linux_events_waiter&) noexcept = delete;
    linux_events_waiter& operator=(const linux_events_waiter&) noexcept = delete;

    linux_events_waiter           (linux_events_waiter&&) noexcept = delete;
    linux_events_waiter& operator=(linux_events_waiter&&) noexcept = delete;

public:
    GKR_CORE_API linux_events_waiter(std::size_t flags = 0) noexcept;
    GKR_CORE_API virtual ~linux_events_waiter() noexcept;

private:
    alignas(void*)
    mutable char      m_mutex[40] {};
    std::vector<int>  m_events    {};
    wait_result_t     m_auto_mask       {0};
    std::size_t       m_waiting_threads {0};

public:
    GKR_CORE_API virtual std::size_t events_count() const noexcept override;

    GKR_CORE_API virtual bool pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual bool remove_all_events() noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_CORE_API virtual bool add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false) override;

    GKR_CORE_API virtual bool fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual bool reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

private:
    GKR_CORE_API virtual wait_result_t check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual wait_result_t wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT) override;

public:
    GKR_CORE_API virtual wait_result_t check_all_events() noexcept(DIAG_NOEXCEPT) override;

    GKR_CORE_API virtual wait_result_t wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT) override;

private:
    bool this_thread_can_wait() const noexcept;

    wait_result_t check_events() const noexcept;

    wait_result_t wait_events(long long nsec) noexcept(DIAG_NOEXCEPT);

    wait_result_t check_event(std::size_t index) const noexcept;

    wait_result_t wait_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT);
};

using os_events_waiter = linux_events_waiter;

#endif
}
