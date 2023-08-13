#include <terrau/math.h>

uint8_t terrau_clamp_u8(uint8_t val, uint8_t min, uint8_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

uint16_t terrau_clamp_u16(uint16_t val, uint16_t min, uint16_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

uint32_t terrau_clamp_u32(uint32_t val, uint32_t min, uint32_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

uint64_t terrau_clamp_u64(uint64_t val, uint64_t min, uint64_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

int8_t terrau_clamp_i8(int8_t val, int8_t min, int8_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

int16_t terrau_clamp_i16(int16_t val, int16_t min, int16_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

int32_t terrau_clamp_i32(int32_t val, int32_t min, int32_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}

int64_t terrau_clamp_i64(int64_t val, int64_t min, int64_t max) {
    if (min < val) {
        if (val < max) {
            return val;
        } else {
            return max;
        }
    } else {
        return min;
    }
}
