#include <timer_impl.hpp>


namespace fade {

timer_impl::timer_impl() :
	has_paused_(false),
	stopped_(false)
{}

void timer_impl::start()
{
	reset();
}

void timer_impl::reset()
{
	start_ = pause_ = resume_ = hrc::now();
	has_paused_ = false;
	stopped_ = false;
}

void timer_impl::stop()
{
	stop_ = hrc::now();
	stopped_ = true;
}

void timer_impl::pause()
{
	pause_ = hrc::now();
	has_paused_ = true;
	resume_ = hrc::now();
}

void timer_impl::resume()
{
	resume_ = hrc::now();
}

double timer_impl::elapsed() const
{
	std::chrono::duration<double, std::centi> duration;
	if (has_paused_)
	{
		if (stopped_)
		{
			duration = stop_ - resume_ + (pause_ - start_);
		}
		else
		{
			duration = hrc::now() - resume_ + (pause_ - start_);
		}
	}		

	if (stopped_)
	{
		duration = stop_ - start_;
	}
	else
	{
		duration = hrc::now() - start_;
	}

	return duration.count();
}

std::unique_ptr<timer> fade::get_timer()
{
	return std::make_unique<timer_impl>();
}

}
