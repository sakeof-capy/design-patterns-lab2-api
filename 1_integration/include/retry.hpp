#ifndef RETRY_HPP_
#define RETRY_HPP_

#include <cstddef>
#include <functional>
#include <httplib.h>
#include <optional>

class Retry
{
public:
    explicit Retry(std::function<httplib::Result()>&& request_fn);

public:
    Retry& with_base_delay_ms(std::size_t base_delay);
    Retry& with_max_delay_ms(std::size_t max_delay);
    Retry& with_max_retries(std::size_t max_retries);
    std::optional<httplib::Result> send();

private:
    std::function<httplib::Result()> request_fn_;
    std::size_t base_delay_ms_   { DEFAULT_BASE_DELAY_MS };
    std::size_t max_delay_ms_    { DEFAULT_MAX_DELAY_MS };
    std::size_t max_retries_     { DEFAULT_MAX_RETRIES };

private:
    static constexpr std::size_t DEFAULT_BASE_DELAY_MS = 500;
    static constexpr std::size_t DEFAULT_MAX_DELAY_MS = 5000;
    static constexpr std::size_t DEFAULT_MAX_RETRIES = 5;
};

#endif //RETRY_HPP_
