#include <core/timer.hpp>
#include <chrono>

namespace fade {

using hrc = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

class timer::impl
{
public:
    hrc::time_point start_, pause_, resume_, stop_;
    bool has_paused_;
    bool stopped_;
};

timer::timer() :
    FADE_INIT_PIMPL(timer)
{
    impl_->has_paused_ = false;
    impl_->stopped_ = false;
}

timer::~timer()
{
}

void timer::start()
{
	reset();
}

void timer::reset()
{
	impl_->start_ = impl_->pause_ = impl_->resume_ = hrc::now();
	impl_->has_paused_ = false;
	impl_->stopped_ = false;
}

void timer::stop()
{
	impl_->stop_ = hrc::now();
	impl_->stopped_ = true;
}

void timer::pause()
{
	impl_->pause_ = hrc::now();
	impl_->has_paused_ = true;
	impl_->resume_ = hrc::now();
}

void timer::resume()
{
	impl_->resume_ = hrc::now();
}

double timer::elapsed() const
{
	std::chrono::duration<double, std::centi> duration;
	if (impl_->has_paused_)
	{
		if (impl_->stopped_)
		{
			duration = impl_->stop_ - impl_->resume_ + (impl_->pause_ - impl_->start_);
		}
		else
		{
			duration = hrc::now() - impl_->resume_ + (impl_->pause_ - impl_->start_);
		}

        return duration.count();
	}		

	if (impl_->stopped_)
	{
		duration = impl_->stop_ - impl_->start_;
	}
	else
	{
		duration = hrc::now() - impl_->start_;
	}

	return duration.count();
}

}
