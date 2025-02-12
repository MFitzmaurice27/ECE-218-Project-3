#include "mbed.h"
#include "arm_book_lib.h"

#include "car_system.h"

int main()
{

    carSystemInit();

    while (true) {

        carSystemUpdate();

    }
}
