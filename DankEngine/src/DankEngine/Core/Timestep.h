#pragma once

namespace Dank {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : _time(time)
		{

		}

		// Allows implicit float usecase
		operator float() const { return _time; }

		float GetSeconds() const { return _time; }
		float GetMilliseconds() const { return _time * 1000.0f; }
	private:
		float _time;
	};
}
