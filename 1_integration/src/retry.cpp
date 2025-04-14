#include "retry.hpp"
#include <random>
#include <chrono>
#include <thread>
#include <optional>
#include <httplib.h>

// equal jitter (is more predictable compared to equal jitter)
static inline std::size_t jittered_delay(
    const std::size_t base_delay_ms,
    const std::size_t attempt,
    const std::size_t max_delay_ms
) {
    const std::size_t exponential = base_delay_ms << attempt; // 2^attempt
    const std::size_t capped = std::min(exponential, max_delay_ms);
    const std::size_t half_capped = capped >> 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random(0, half_capped);

    return half_capped + random(gen);
}

Retry::Retry(std::function<httplib::Result()>&& request_fn)
    : request_fn_ { std::move(request_fn) }
{}

Retry& Retry::with_base_delay_ms(std::size_t base_delay)
{
    this->base_delay_ms_ = base_delay;
    return *this;
}

Retry& Retry::with_max_delay_ms(std::size_t max_delay)
{
    this->max_delay_ms_ = max_delay;
    return *this;
}

Retry& Retry::with_max_retries(std::size_t max_retries)
{
    this->max_retries_ = max_retries;
    return *this;
}

std::optional<httplib::Result> Retry::send()
{
    const auto should_be_retried = [](const int error_code) {
        static constexpr std::array retriable_codes = { 408, 425, 429, 500, 502, 503, 504 };
        return std::find(retriable_codes.cbegin(), retriable_codes.cend(), error_code) != retriable_codes.cend();
    };

    for (std::size_t attempt = 0; attempt <= this->max_retries_; ++attempt)
    {
        httplib::Result result = this->request_fn_();

        if (result && !should_be_retried(result->status))
        {
            return result;
        }

        if (attempt < this->max_retries_)
        {
            const std::size_t delay = jittered_delay(this->base_delay_ms_, attempt, this->max_delay_ms_);
            std::cout << "[Retry " << attempt + 1 << " (status=" << result->status <<")] Waiting " << delay << "ms...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    return std::nullopt;
}
