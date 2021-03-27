#include <Arduino.h>
#include <SEGGER_RTT.h>
#include <loading.h>

extern "C" {

void do_something();

void do_another();

}

extern "C" char *sbrk(int32_t i);

static void (*callback)() = do_something;

class Base {
public:
    Base() {
        // SEGGER_RTT_WriteString(0, "Base::Base\n");
    }

    virtual ~Base() {
        // SEGGER_RTT_WriteString(0, "Base::~Base\n");
    }

public:
    virtual void hello() {
        SEGGER_RTT_WriteString(0, "Base::hello\n");
    }
    virtual void bye() {
        SEGGER_RTT_WriteString(0, "Base::bye\n");
    }
};

class Derived : public Base  {
public:
    Derived() {
        // SEGGER_RTT_WriteString(0, "Derived::Derived!\n");
    }

    virtual ~Derived() {
        // SEGGER_RTT_WriteString(0, "Derived::~Derived!\n");
    }

public:
    void hello() override {
        Base::hello();
        SEGGER_RTT_WriteString(0, "Derived::hello\n");
    }
    void bye() override {
        Base::bye();
        SEGGER_RTT_WriteString(0, "Derived::bye\n");
    }
};

void work(uint32_t i) {
    if (random(0, 100) > 50) {
        callback = do_another;
    }
    else {
        callback = do_something;
    }
}

void test_object() {
    auto obj = new Derived();
    obj->hello();
    // obj->bye();
}

void setup() {
    test_object();

    callback();

    // Prevent the compiler from optimizing this. What's funny is it'll still do
    // the GOT lookup and just bx to whatever you assign if you make this more
    // predictable.
    work(20);

    callback();

    SEGGER_RTT_WriteString(0, "\n\n");
    SEGGER_RTT_WriteString(0, "Hello!\n");

    while (true) {
        delay(1000);
        SEGGER_RTT_WriteString(0, "PING\n");
    }
}

void loop() {

}
