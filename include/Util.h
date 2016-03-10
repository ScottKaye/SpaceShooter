#pragma once

inline int clamp(int value, int low, int high) {
	return value > high
		? high
		: value < low
		? low
		: value;
}

inline float clamp(float value, float low, float high) {
	return value > high
		? high
		: value < low
		? low
		: value;
}