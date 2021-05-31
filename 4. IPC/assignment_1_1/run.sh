#!/bin/sh

gcc message_buffer.h message_buffer.c consumer.c
mv a.out consumer

gcc message_buffer.h message_buffer.c producer.c
mv a.out producer

gcc message_buffer.h message_buffer.c destroy.c
mv a.out destroy