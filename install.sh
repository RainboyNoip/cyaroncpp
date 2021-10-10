#!/bin/sh
#install
sudo rm -rf /usr/include/cyaroncpp/
sudo mkdir -p /usr/include/cyaroncpp/
sudo cp -r ./include /usr/include/cyaroncpp/
sudo cp -r ./cyaron.hpp /usr/include/cyaroncpp/
sudo chmod -R 755 /usr/include/cyaroncpp/
