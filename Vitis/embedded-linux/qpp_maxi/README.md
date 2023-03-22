# Contents
1. Linux set-up
2. SSH over Ethernet
3. Program device

# Linux set-up
Steven Knudsen has put together a comprehensive tutorial on building a Linux kernel for the Zynq-7000 using the PetaLinux CLI tools. This tutorial is available on his Github repository. https://github.com/StevenKnudsen/PetaLinux2022.2_Zybo_example/blob/main/docs/PetaLinux/PetaLinux%20Notes%20Part%202.md </br>
The instructions contained in "PetaLinux Notes Part 02" were followed to build and deploy a kernel to the Zybo-Z20 for this project.
# SSH over Ethernet
1. Connect the Zybo-Z20 with your host machine using an Ethernet cable.
2. Open a serial terminal on your host machine to connect with the Zybo-Z20 as described in "PetaLinux Notes Part 02" linked above.
3. Assign an IP address and subnet mask to the Ethernet interface on the Zybo-Z20.
a) Use ``$ ifconfig`` to list all network interfaces. The Ethernet interface should be named "eth0". </br>
b) Bring down the Ethernet interface: </br>
``$ ifconfig <interface-name> down``</br>
c) Assign a static IP address and subnet mask </br>
``$ ifconfig <interface-name> <ip-address> netmask <subnet-mask>``</br>
eg. </br> ``$ ifconfig eth0 192.168.1.10 netmask 255.255.255.0``</br>
d) Bring the Ethernet interface back up: </br>
``$ ifconfig <interface-name> up`` </br>
4. Assign an IP address and subnet mask to the Ethernet interface on your host machine. </br>
If your hostmachine is running a Linux OS, you can follow the same steps as above. Make sure to select a different static IP address than was assigned to the Zybo-Z20. The Ethernet interface name may be different on your machine, such as ``enp3s0`` instead of ``eth0``. Make sure to check your Ethernet interface name using ``ifconfig``. If you shell does not have root access, you will need to preface the ifconfig commands with ``sudo``. </br>
eg.
```
$ sudo ifconfig enp3s0 down
$ sudo ifconfig enp3s0 192.168.1.11 netmask 255.255.255.0
$ sudo ifconfig enp3s0 up
```
5. Verify the connection such as using the ``ping`` command.
eg.
From the host machine terminal:
``ping 192.168.1.0`` </br>
You should see similar output in your terminal as below. </br>
```
PING 192.168.1.10 (192.168.1.10) 56(84) bytes of data.
64 bytes from 192.168.1.10: icmp_seq=1 ttl=64 time=1.27 ms
64 bytes from 192.168.1.10: icmp_seq=2 ttl=64 time=0.597 ms
64 bytes from 192.168.1.10: icmp_seq=3 ttl=64 time=0.409 ms
64 bytes from 192.168.1.10: icmp_seq=4 ttl=64 time=0.402 ms
```
