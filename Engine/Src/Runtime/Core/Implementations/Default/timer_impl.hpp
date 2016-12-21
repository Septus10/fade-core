#pragma once

#include <timer.hpp>

#include <chrono>

namespace fade
{
	
class timer_impl : public timer
{
	using hrc = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;
public:
	timer_impl();

	~timer_impl() override = default;

	void start() override;

	void reset() override;

	void stop() override;

	void pause() override;

	void resume() override;

	double elapsed() const override;

private:
	hrc::time_point start_, pause_, resume_, stop_;
	bool has_paused_;
	bool stopped_;

};

}