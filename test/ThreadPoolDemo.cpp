/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#include <iostream>
#include "rtps/rtps.h"
#include "rtps/entities/Domain.h"

#define PUB 1

void receiveCallback(void* /*callee*/, rtps::ReaderCacheChange& /*cacheChange*/){
    printf("Received hello world message.\n");
}

void startProgram(void*);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main() {
    rtps::init();

    sys_thread_new("Main Program", startProgram, nullptr, 1024, 3);
    while(true);
}

void startProgram(void* /*args*/){
    rtps::Domain domain;
    domain.start();

    auto part = domain.createParticipant();

    if(part == nullptr){
        printf("Failed to create participant");
        return;
    }

    char topicName[] = "HelloWorldTopic";
    char typeName[] = "HelloWorld";

    //auto part2 = domain.createParticipant();
    //rtps::Reader* reader = domain.createReader(*part2, topicName, typeName, true);
    //reader->registerCallback(receiveCallback, nullptr);

#if PUB
    rtps::Writer* writer = domain.createWriter(*part, topicName, typeName, true);
    if(writer == nullptr){
        printf("Failed to create writer");
        return;
    }

    char message[] = "Hello World";
    uint8_t buffer[16];
    ucdrBuffer microbuffer;
    uint32_t i = 0;
    while(true){
        ucdr_init_buffer(&microbuffer, buffer, sizeof(buffer)/ sizeof(buffer[0]));
        ucdr_serialize_uint32_t(&microbuffer, i);
        ucdr_serialize_array_char(&microbuffer, message, sizeof(message)/sizeof(message[0]));
        auto change = writer->newChange(rtps::ChangeKind_t::ALIVE, buffer, sizeof(buffer)/ sizeof(buffer[0]));
        if(change == nullptr){
            printf("History full.\n");
        }
        sys_msleep(5000);

    }

#else

    rtps::Reader* reader = domain.createReader(*part, topicName, typeName, true);
    reader->registerCallback(receiveCallback, nullptr);

    while(true){
        // Nothing to do. Just wait...
    }

#endif
}

#pragma clang diagnostic pop