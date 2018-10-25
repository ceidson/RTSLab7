#include <chrono>
#include "time_util.h"

using namespace std::chrono;

int current_timestamp() {
	milliseconds end = duration_cast < milliseconds > (system_clock::now().time_since_epoch());
	return end.count();
}
