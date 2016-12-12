#include "infrared-send.h"

int main(void) {
    ir_setup();

    sendDataOld(0x43); sendDataOld(0x43); sendDataOld(0x43); sendDataOld(0x43); 
    sendDataOld(0x43); sendDataOld(0x43); sendDataOld(0x43); sendDataOld(0x43); 

    for(;;){
        sendData(0x41);
    }
}
