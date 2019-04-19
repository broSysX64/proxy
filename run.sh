#!/bin/bash

./build/main/app -t postgres --local-host 127.0.0.1 --local-port 7777 --forward-host 127.0.0.1 --forward-port 5432

