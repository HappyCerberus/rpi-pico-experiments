# Button to LED

```c++
std::array<int, 4> in {18, 19, 20, 21};
std::array<int, 4> out {10, 11, 12, 13};

for (int gpio_pin : in) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
}

for (int gpio_pin : out) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
}

while (true) {
    for (int i = 0; i < 4; i++) {
        if (gpio_get(in[i]) == 1) {
            gpio_pull_up(out[i]);
        } else {
            gpio_pull_down(out[i]);
        }
    }
    
    printf(".");
    sleep_ms(200);
}
```