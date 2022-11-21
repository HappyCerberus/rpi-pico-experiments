#include <cstdio>
#include <array>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

bool timeout_expired(absolute_time_t start_time, int64_t timeout) {
    return absolute_time_diff_us(start_time,get_absolute_time()) > timeout;
}

int64_t pulse_in(uint gpio, int64_t timeout = 1'000'000) {
    gpio_set_dir(gpio, GPIO_IN);
    absolute_time_t t = get_absolute_time();
    // Wait for low
    while (gpio_get(gpio))
        if (timeout_expired(t, timeout))
            return -1;
    // Wait for high
    while (!gpio_get(gpio))
        if (timeout_expired(t, timeout))
            return -2;
    // this is the leading edge, remember timestamp
    absolute_time_t signal = get_absolute_time();
    // wait for low
    while (gpio_get(gpio))
        if (timeout_expired(t, timeout))
            return -3;
    // return the duration of the pulse
    return absolute_time_diff_us(signal, get_absolute_time());
}

void pulse_out(uint gpio, uint64_t length) {
    gpio_set_dir(gpio, GPIO_OUT);
    gpio_pull_down(gpio);
    sleep_us(2);
    gpio_pull_up(gpio);
    sleep_us(length);
    gpio_pull_down(gpio);
}

int main() {
    stdio_init_all();
    gpio_init(16);

    while (true) {
        // Ping the sensor
        pulse_out(16, 10);

        // Read the output
        int64_t pulse = pulse_in(16);
        switch (pulse) {
            case -1: printf("sinal stuck\n"); break;
            case -2: printf("no signal\n"); break;
            case -3: printf("did not end\n"); break;
            default: printf("%lldcm\n", pulse/29/2);
        }
        sleep_ms(200);
    }
}