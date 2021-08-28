#!/bin/sh

#install
#sh -c "$(curl -fsSL https://ghproxy.com/https://github.com/RainboyNoip/cyaroncpp/blob/master/install.sh)"
sudo curl -o /usr/include/cyaroncpp/cyaron.hpp --create-dirs https://ghproxy.com/https://github.com/RainboyNoip/cyaroncpp/blob/master/cyaron.hpp
sudo chmod -R 755 /usr/include/cyaroncpp/
