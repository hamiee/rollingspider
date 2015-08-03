#sudo mkdir ./Parrot_RS
sudo mount -t vfat /dev/sdc1 ./Parrot_RS



sudo cp a.out ./Parrot_RS/main
sudo cp a.out ./Parrot_RS/main1
sudo cp a.out ./Parrot_RS/main2
sudo cp a.out ./Parrot_RS/main3
sudo cp a.out ./Parrot_RS/main5
sudo cp a.out ./Parrot_RS/main4

sudo umount ./Parrot_RS



sudo ifconfig usb0 192.168.2.2 up
#sleep 1
telnet 192.168.2.1

