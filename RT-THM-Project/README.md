# RT-THM - Real-Time Task & Health Monitor

## Overview

RT-THM is a Linux-based monitoring system developed in C using a Supervisor–Workers architecture.

The supervisor creates and monitors worker processes using shared memory and semaphores. If a worker crashes, the supervisor automatically restarts it.

## Features

- Multi-process architecture
- Shared memory communication
- Semaphore synchronization
- Health monitoring
- Auto-healing mechanism
- Signal handling
- Logging system

## Architecture

Supervisor
|
+-- Worker 1
+-- Worker 2
+-- Worker 3

Shared Memory
Semaphores

## Technologies

- C
- Linux
- System V Shared Memory
- Semaphores
- Signals
- Makefile

## Build

make

## Run

./rt_thm

## Dashboard

![Dashboard](screenshots/capture_dashboard.png)


## Author

Zayneb Hachem
ENISO - Automotive Software Engineering
