# CSCD58 A3

## Contributor (solo):

Name: Christine Zhao

UTORid: zhaochr3

Student Number: 1007914786

Email: christine.zhao@mail.utoronto.ca

## Functions Implemented

All required functionalities written in the handout should be implemented.

## Tests Run:

With the router ran with the command `./sr` in the `/router` folder:

The following commands run one after the other:

Ping valid address:
```
mininet> server1 ping -c 3 10.0.1.1
nohup: appending output to 'nohup.out'
PING 10.0.1.1 (10.0.1.1) 56(84) bytes of data.
64 bytes from 10.0.1.1: icmp_seq=1 ttl=255 time=109 ms
64 bytes from 10.0.1.1: icmp_seq=2 ttl=255 time=34.7 ms
64 bytes from 10.0.1.1: icmp_seq=3 ttl=255 time=66.2 ms

--- 10.0.1.1 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2003ms
rtt min/avg/max/mdev = 34.668/69.990/109.106/30.507 ms
```
Ping valid router interface:
```
mininet> server1 ping -c 3 172.64.3.10
PING 172.64.3.10 (172.64.3.10) 56(84) bytes of data.
64 bytes from 172.64.3.10: icmp_seq=1 ttl=63 time=83.4 ms
64 bytes from 172.64.3.10: icmp_seq=2 ttl=63 time=40.2 ms
64 bytes from 172.64.3.10: icmp_seq=3 ttl=63 time=83.2 ms

--- 172.64.3.10 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2003ms
rtt min/avg/max/mdev = 40.199/68.927/83.402/20.313 ms
```

Ping invalid address:
```
mininet> server2 ping -c 3 10.0.0.6
nohup: appending output to 'nohup.out'
PING 10.0.0.6 (10.0.0.6) 56(84) bytes of data.
From 172.64.3.1 icmp_seq=1 Destination Net Unreachable
From 172.64.3.1 icmp_seq=2 Destination Net Unreachable
From 172.64.3.1 icmp_seq=3 Destination Net Unreachable

--- 10.0.0.6 ping statistics ---
3 packets transmitted, 0 received, +3 errors, 100% packet loss, time 2004ms
```

Traceroute valid address:
```
mininet> server1 traceroute client
traceroute to 10.0.1.100 (10.0.1.100), 30 hops max, 60 byte packets
 1  192.168.2.1 (192.168.2.1)  277.829 ms  230.400 ms  215.846 ms
 2  * * *
 3  * * *
 4  * 10.0.1.100 (10.0.1.100)  494.715 ms  484.721 ms
```

Traceroute valid router interface:
```
mininet> client traceroute 172.64.3.1
traceroute to 172.64.3.1 (172.64.3.1), 30 hops max, 60 byte packets
 1  10.0.1.1 (10.0.1.1)  24.768 ms  44.770 ms  62.020 ms
```

Traceroute valid address (using node name):
```
mininet> server2 traceroute server1
traceroute to 192.168.2.2 (192.168.2.2), 30 hops max, 60 byte packets
 1  172.64.3.1 (172.64.3.1)  270.727 ms  270.664 ms  222.503 ms
 2  * * *
 3  * * *
 4  192.168.2.2 (192.168.2.2)  488.941 ms *  481.016 ms
```

Valid wget:
```
mininet> client wget http://192.168.2.2
--2024-10-30 10:41:15--  http://192.168.2.2/
Connecting to 192.168.2.2:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 161 [text/html]
Saving to: 'index.html'

index.html          100%[===================>]     161  --.-KB/s    in 0s

2024-10-30 10:41:15 (40.9 MB/s) - 'index.html' saved [161/161]
```

Valid wget:
```
mininet> server1 wget http://192.168.2.2
--2024-10-30 10:41:19--  http://192.168.2.2/
Connecting to 192.168.2.2:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 161 [text/html]
Saving to: 'index.html.9'

index.html.9        100%[===================>]     161  --.-KB/s    in 0s

2024-10-30 10:41:19 (36.4 MB/s) - 'index.html.9' saved [161/161]
```

Multiple traceroutes:
```
mininet> server2 traceroute server1
traceroute to 192.168.2.2 (192.168.2.2), 30 hops max, 60 byte packets
 1  172.64.3.1 (172.64.3.1)  271.784 ms  259.041 ms  243.241 ms
 2  * * *
 3  * * *
 4  * 192.168.2.2 (192.168.2.2)  506.623 ms  498.624 ms
```

```
mininet> server2 traceroute server1
traceroute to 192.168.2.2 (192.168.2.2), 30 hops max, 60 byte packets
 1  172.64.3.1 (172.64.3.1)  65.569 ms  110.462 ms  110.482 ms
 2  192.168.2.2 (192.168.2.2)  373.738 ms  384.184 ms  394.118 ms

```

Invalid traceroute address:

```
mininet> server2 traceroute 10.0.2.2
traceroute to 10.0.2.2 (10.0.2.2), 30 hops max, 60 byte packets
 1  172.64.3.1 (172.64.3.1)  358.234 ms  339.656 ms  312.936 ms
 2  172.64.3.1 (172.64.3.1)  302.697 ms !N  296.058 ms !N  288.696 ms !N
```

Which produced the following tcpdump .pcap file:

```
reading from file expectedoutput.pcap, link-type EN10MB (Ethernet)
10:40:16.493942 ARP, Request who-has 192.168.2.1 tell 192.168.2.2, length 28
10:40:16.493980 ARP, Reply 192.168.2.1 is-at b2:02:8f:e7:e4:fc (oui Unknown), length 28
10:40:16.516807 IP 192.168.2.2 > 10.0.1.1: ICMP echo request, id 46167, seq 1, length 64
10:40:16.516841 ARP, Request who-has 192.168.2.2 (Broadcast) tell 192.168.2.1, length 28
10:40:16.587213 ARP, Reply 192.168.2.2 is-at be:14:05:22:c9:1f (oui Unknown), length 28
10:40:16.587244 IP 10.0.1.1 > 192.168.2.2: ICMP echo reply, id 46167, seq 1, length 64
10:40:17.513959 IP 192.168.2.2 > 10.0.1.1: ICMP echo request, id 46167, seq 2, length 64
10:40:17.513995 IP 10.0.1.1 > 192.168.2.2: ICMP echo reply, id 46167, seq 2, length 64
10:40:18.545576 IP 192.168.2.2 > 10.0.1.1: ICMP echo request, id 46167, seq 3, length 64
10:40:18.545614 IP 10.0.1.1 > 192.168.2.2: ICMP echo reply, id 46167, seq 3, length 64
10:40:24.393595 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 1, length 64
10:40:24.393623 ARP, Request who-has 172.64.3.10 (Broadcast) tell 172.64.3.1, length 28
10:40:24.411210 ARP, Reply 172.64.3.10 is-at 36:52:e8:f2:55:88 (oui Unknown), length 28
10:40:24.411245 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 1, length 64
10:40:24.429121 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 1, length 64
10:40:24.429167 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 1, length 64
10:40:25.401573 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 2, length 64
10:40:25.401610 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 2, length 64
10:40:25.419087 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 2, length 64
10:40:25.419123 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 2, length 64
10:40:26.442992 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 3, length 64
10:40:26.443027 IP 192.168.2.2 > 172.64.3.10: ICMP echo request, id 46201, seq 3, length 64
10:40:26.462666 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 3, length 64
10:40:26.462716 IP 172.64.3.10 > 192.168.2.2: ICMP echo reply, id 46201, seq 3, length 64
10:40:29.487568 IP 172.64.3.10 > 10.0.0.6: ICMP echo request, id 46219, seq 1, length 64
10:40:29.487600 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.0.6 unreachable, length 36
10:40:29.487615 ARP, Request who-has 10.0.0.6 (Broadcast) tell 172.64.3.1, length 28
10:40:29.514195 ARP, Request who-has 172.64.3.1 tell 172.64.3.10, length 28
10:40:29.514241 ARP, Reply 172.64.3.1 is-at f6:08:d1:c6:f5:64 (oui Unknown), length 28
10:40:29.656330 ARP, Request who-has 10.0.0.6 (Broadcast) tell 172.64.3.1, length 28
10:40:30.479526 IP 172.64.3.10 > 10.0.0.6: ICMP echo request, id 46219, seq 2, length 64
10:40:30.479569 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.0.6 unreachable, length 36
10:40:30.656514 ARP, Request who-has 10.0.0.6 (Broadcast) tell 172.64.3.1, length 28
10:40:31.459509 IP 172.64.3.10 > 10.0.0.6: ICMP echo request, id 46219, seq 3, length 64
10:40:31.459544 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.0.6 unreachable, length 36
10:40:31.656714 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.0.6 unreachable, length 36
10:40:31.656769 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.0.6 unreachable, length 36
10:40:31.656780 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.0.6 unreachable, length 36
10:40:35.392662 IP 192.168.2.2.54859 > 10.0.1.100.33434: UDP, length 32
10:40:35.392699 ARP, Request who-has 192.168.2.2 (Broadcast) tell 192.168.2.1, length 28
10:40:35.407327 IP 192.168.2.2.42548 > 10.0.1.100.33435: UDP, length 32
10:40:35.454681 IP 192.168.2.2.36815 > 10.0.1.100.33436: UDP, length 32
10:40:35.454724 IP 192.168.2.2.41595 > 10.0.1.100.33437: UDP, length 32
10:40:35.454729 ARP, Request who-has 10.0.1.100 (Broadcast) tell 10.0.1.1, length 28
10:40:35.454742 IP 192.168.2.2.49177 > 10.0.1.100.33438: UDP, length 32
10:40:35.454758 IP 192.168.2.2.45202 > 10.0.1.100.33439: UDP, length 32
10:40:35.480347 IP 192.168.2.2.39246 > 10.0.1.100.33440: UDP, length 32
10:40:35.480431 IP 192.168.2.2.39137 > 10.0.1.100.33441: UDP, length 32
10:40:35.482483 IP 192.168.2.2.47734 > 10.0.1.100.33442: UDP, length 32
10:40:35.486634 IP 192.168.2.2.53097 > 10.0.1.100.33443: UDP, length 32
10:40:35.503367 IP 192.168.2.2.41609 > 10.0.1.100.33444: UDP, length 32
10:40:35.508414 IP 192.168.2.2.47906 > 10.0.1.100.33445: UDP, length 32
10:40:35.524711 IP 192.168.2.2.48409 > 10.0.1.100.33446: UDP, length 32
10:40:35.529312 IP 192.168.2.2.34136 > 10.0.1.100.33447: UDP, length 32
10:40:35.556338 IP 192.168.2.2.37244 > 10.0.1.100.33448: UDP, length 32
10:40:35.556375 IP 192.168.2.2.34862 > 10.0.1.100.33449: UDP, length 32
10:40:35.557886 ARP, Reply 192.168.2.2 is-at be:14:05:22:c9:1f (oui Unknown), length 28
10:40:35.557905 IP 192.168.2.1 > 192.168.2.2: ICMP time exceeded in-transit, length 36
10:40:35.557912 IP 192.168.2.1 > 192.168.2.2: ICMP time exceeded in-transit, length 36
10:40:35.557915 IP 192.168.2.1 > 192.168.2.2: ICMP time exceeded in-transit, length 36
10:40:35.602620 ARP, Reply 10.0.1.100 is-at d6:69:9b:9b:35:f5 (oui Unknown), length 28
10:40:35.602654 IP 192.168.2.2.34862 > 10.0.1.100.33449: UDP, length 32
10:40:35.602662 IP 192.168.2.2.37244 > 10.0.1.100.33448: UDP, length 32
10:40:35.602665 IP 192.168.2.2.34136 > 10.0.1.100.33447: UDP, length 32
10:40:35.602667 IP 192.168.2.2.48409 > 10.0.1.100.33446: UDP, length 32
10:40:35.602669 IP 192.168.2.2.47906 > 10.0.1.100.33445: UDP, length 32
10:40:35.602672 IP 192.168.2.2.41609 > 10.0.1.100.33444: UDP, length 32
10:40:35.602674 IP 192.168.2.2.53097 > 10.0.1.100.33443: UDP, length 32
10:40:35.602676 IP 192.168.2.2.47734 > 10.0.1.100.33442: UDP, length 32
10:40:35.602678 IP 192.168.2.2.39137 > 10.0.1.100.33441: UDP, length 32
10:40:35.602680 IP 192.168.2.2.39246 > 10.0.1.100.33440: UDP, length 32
10:40:35.602682 IP 192.168.2.2.45202 > 10.0.1.100.33439: UDP, length 32
10:40:35.602684 IP 192.168.2.2.49177 > 10.0.1.100.33438: UDP, length 32
10:40:35.602686 IP 192.168.2.2.41595 > 10.0.1.100.33437: UDP, length 32
10:40:35.602690 IP 192.168.2.2.37872 > 10.0.1.100.33450: UDP, length 32
10:40:35.602694 IP 192.168.2.2.37872 > 10.0.1.100.33450: UDP, length 32
10:40:35.602698 IP 192.168.2.2.45631 > 10.0.1.100.33451: UDP, length 32
10:40:35.602701 IP 192.168.2.2.45631 > 10.0.1.100.33451: UDP, length 32
10:40:35.786615 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33449 unreachable, length 68
10:40:35.786654 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33449 unreachable, length 68
10:40:35.786667 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33448 unreachable, length 68
10:40:35.786670 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33448 unreachable, length 68
10:40:35.786690 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33447 unreachable, length 68
10:40:35.786702 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33447 unreachable, length 68
10:40:35.786707 IP 192.168.2.2.56795 > 10.0.1.100.33452: UDP, length 32
10:40:35.786710 IP 192.168.2.2.56795 > 10.0.1.100.33452: UDP, length 32
10:40:35.786713 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33446 unreachable, length 68
10:40:35.786716 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33446 unreachable, length 68
10:40:35.786719 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33445 unreachable, length 68
10:40:35.786742 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33445 unreachable, length 68
10:40:35.786745 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33444 unreachable, length 68
10:40:35.786748 IP 10.0.1.100 > 192.168.2.2: ICMP 10.0.1.100 udp port 33444 unreachable, length 68
10:40:40.782417 ARP, Request who-has 10.0.1.1 tell 10.0.1.100, length 28
10:40:40.782454 ARP, Reply 10.0.1.1 is-at f2:5f:1f:d4:a8:05 (oui Unknown), length 28
10:40:42.065156 IP 10.0.1.100.43192 > 172.64.3.1.33434: UDP, length 32
10:40:42.065191 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33434 unreachable, length 36
10:40:42.080399 IP 10.0.1.100.47144 > 172.64.3.1.33435: UDP, length 32
10:40:42.080455 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33435 unreachable, length 36
10:40:42.097649 IP 10.0.1.100.40087 > 172.64.3.1.33436: UDP, length 32
10:40:42.097696 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33436 unreachable, length 36
10:40:42.117641 IP 10.0.1.100.52800 > 172.64.3.1.33437: UDP, length 32
10:40:42.117675 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33437 unreachable, length 36
10:40:42.117690 IP 10.0.1.100.56957 > 172.64.3.1.33438: UDP, length 32
10:40:42.117703 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33438 unreachable, length 36
10:40:42.161143 IP 10.0.1.100.47689 > 172.64.3.1.33439: UDP, length 32
10:40:42.161175 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33439 unreachable, length 36
10:40:42.161189 IP 10.0.1.100.35743 > 172.64.3.1.33440: UDP, length 32
10:40:42.161194 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33440 unreachable, length 36
10:40:42.161203 IP 10.0.1.100.46724 > 172.64.3.1.33441: UDP, length 32
10:40:42.161206 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33441 unreachable, length 36
10:40:42.217546 IP 10.0.1.100.37822 > 172.64.3.1.33442: UDP, length 32
10:40:42.217578 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33442 unreachable, length 36
10:40:42.217593 IP 10.0.1.100.52418 > 172.64.3.1.33443: UDP, length 32
10:40:42.217617 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33443 unreachable, length 36
10:40:42.217628 IP 10.0.1.100.60722 > 172.64.3.1.33444: UDP, length 32
10:40:42.217632 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33444 unreachable, length 36
10:40:42.217639 IP 10.0.1.100.54245 > 172.64.3.1.33445: UDP, length 32
10:40:42.217642 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33445 unreachable, length 36
10:40:42.217647 IP 10.0.1.100.54660 > 172.64.3.1.33446: UDP, length 32
10:40:42.217650 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33446 unreachable, length 36
10:40:42.272880 IP 10.0.1.100.60190 > 172.64.3.1.33447: UDP, length 32
10:40:42.272918 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33447 unreachable, length 36
10:40:42.272931 IP 10.0.1.100.33550 > 172.64.3.1.33448: UDP, length 32
10:40:42.272935 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33448 unreachable, length 36
10:40:42.272940 IP 10.0.1.100.45062 > 172.64.3.1.33449: UDP, length 32
10:40:42.272943 IP 10.0.1.1 > 10.0.1.100: ICMP 172.64.3.1 udp port 33449 unreachable, length 36
10:40:59.302280 IP 172.64.3.10.55809 > 192.168.2.2.33434: UDP, length 32
10:40:59.302320 ARP, Request who-has 172.64.3.10 (Broadcast) tell 172.64.3.1, length 28
10:40:59.316603 IP 172.64.3.10.52855 > 192.168.2.2.33435: UDP, length 32
10:40:59.364812 IP 172.64.3.10.35533 > 192.168.2.2.33436: UDP, length 32
10:40:59.364858 IP 172.64.3.10.38398 > 192.168.2.2.33437: UDP, length 32
10:40:59.364864 ARP, Request who-has 192.168.2.2 (Broadcast) tell 192.168.2.1, length 28
10:40:59.364877 IP 172.64.3.10.41227 > 192.168.2.2.33438: UDP, length 32
10:40:59.364882 IP 172.64.3.10.37491 > 192.168.2.2.33439: UDP, length 32
10:40:59.392249 IP 172.64.3.10.52122 > 192.168.2.2.33440: UDP, length 32
10:40:59.392294 IP 172.64.3.10.51969 > 192.168.2.2.33441: UDP, length 32
10:40:59.393202 IP 172.64.3.10.55012 > 192.168.2.2.33442: UDP, length 32
10:40:59.397869 IP 172.64.3.10.46409 > 192.168.2.2.33443: UDP, length 32
10:40:59.418613 IP 172.64.3.10.49286 > 192.168.2.2.33445: UDP, length 32
10:40:59.434344 IP 172.64.3.10.35555 > 192.168.2.2.33446: UDP, length 32
10:40:59.439127 IP 172.64.3.10.41932 > 192.168.2.2.33447: UDP, length 32
10:40:59.454300 IP 172.64.3.10.40940 > 192.168.2.2.33448: UDP, length 32
10:40:59.458794 IP 172.64.3.10.54480 > 192.168.2.2.33449: UDP, length 32
10:40:59.475511 ARP, Reply 172.64.3.10 is-at 36:52:e8:f2:55:88 (oui Unknown), length 28
10:40:59.475545 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:40:59.475552 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:40:59.475556 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:40:59.513748 ARP, Reply 192.168.2.2 is-at be:14:05:22:c9:1f (oui Unknown), length 28
10:40:59.513798 IP 172.64.3.10.54480 > 192.168.2.2.33449: UDP, length 32
10:40:59.513806 IP 172.64.3.10.40940 > 192.168.2.2.33448: UDP, length 32
10:40:59.513810 IP 172.64.3.10.41932 > 192.168.2.2.33447: UDP, length 32
10:40:59.513813 IP 172.64.3.10.35555 > 192.168.2.2.33446: UDP, length 32
10:40:59.513815 IP 172.64.3.10.49286 > 192.168.2.2.33445: UDP, length 32
10:40:59.513818 IP 172.64.3.10.46409 > 192.168.2.2.33443: UDP, length 32
10:40:59.513820 IP 172.64.3.10.55012 > 192.168.2.2.33442: UDP, length 32
10:40:59.513823 IP 172.64.3.10.51969 > 192.168.2.2.33441: UDP, length 32
10:40:59.513825 IP 172.64.3.10.52122 > 192.168.2.2.33440: UDP, length 32
10:40:59.513827 IP 172.64.3.10.37491 > 192.168.2.2.33439: UDP, length 32
10:40:59.513829 IP 172.64.3.10.41227 > 192.168.2.2.33438: UDP, length 32
10:40:59.513831 IP 172.64.3.10.38398 > 192.168.2.2.33437: UDP, length 32
10:40:59.513836 IP 172.64.3.10.58216 > 192.168.2.2.33450: UDP, length 32
10:40:59.513841 IP 172.64.3.10.58216 > 192.168.2.2.33450: UDP, length 32
10:40:59.677489 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33449 unreachable, length 68
10:40:59.677524 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33449 unreachable, length 68
10:40:59.677538 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33448 unreachable, length 68
10:40:59.677542 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33448 unreachable, length 68
10:40:59.677546 IP 172.64.3.10.38182 > 192.168.2.2.33451: UDP, length 32
10:40:59.677549 IP 172.64.3.10.38182 > 192.168.2.2.33451: UDP, length 32
10:40:59.677553 IP 172.64.3.10.38484 > 192.168.2.2.33452: UDP, length 32
10:40:59.677555 IP 172.64.3.10.38484 > 192.168.2.2.33452: UDP, length 32
10:40:59.677558 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33447 unreachable, length 68
10:40:59.677561 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33447 unreachable, length 68
10:40:59.677564 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33446 unreachable, length 68
10:40:59.677566 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33446 unreachable, length 68
10:40:59.677569 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33445 unreachable, length 68
10:40:59.677572 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33445 unreachable, length 68
10:40:59.677575 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33443 unreachable, length 68
10:40:59.677578 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33443 unreachable, length 68
10:41:04.335318 ARP, Request who-has 172.64.3.1 tell 172.64.3.10, length 28
10:41:04.335353 ARP, Reply 172.64.3.1 is-at f6:08:d1:c6:f5:64 (oui Unknown), length 28
10:41:04.814779 ARP, Request who-has 192.168.2.1 tell 192.168.2.2, length 28
10:41:04.814820 ARP, Reply 192.168.2.1 is-at b2:02:8f:e7:e4:fc (oui Unknown), length 28
10:41:15.133836 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [S], seq 1185193469, win 42340, options [mss 1460,sackOK,TS val 3098599567 ecr 0,nop,wscale 9], length 0
10:41:15.133888 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [S], seq 1185193469, win 42340, options [mss 1460,sackOK,TS val 3098599567 ecr 0,nop,wscale 9], length 0
10:41:15.149206 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [S.], seq 1284555735, ack 1185193470, win 43440, options [mss 1460,sackOK,TS val 533959573 ecr 3098599567,nop,wscale 9], length 0
10:41:15.149262 ARP, Request who-has 10.0.1.100 (Broadcast) tell 10.0.1.1, length 28
10:41:15.164465 ARP, Reply 10.0.1.100 is-at d6:69:9b:9b:35:f5 (oui Unknown), length 28
10:41:15.164499 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [S.], seq 1284555735, ack 1185193470, win 43440, options [mss 1460,sackOK,TS val 533959573 ecr 3098599567,nop,wscale 9], length 0
10:41:15.186142 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [.], ack 1, win 83, options [nop,nop,TS val 3098599657 ecr 533959573], length 0
10:41:15.186173 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [.], ack 1, win 83, options [nop,nop,TS val 3098599657 ecr 533959573], length 0
10:41:15.200966 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [P.], seq 1:139, ack 1, win 83, options [nop,nop,TS val 3098599658 ecr 533959573], length 138: HTTP: GET / HTTP/1.1
10:41:15.201009 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [P.], seq 1:139, ack 1, win 83, options [nop,nop,TS val 3098599658 ecr 533959573], length 138: HTTP: GET / HTTP/1.1
10:41:15.236224 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [.], ack 139, win 85, options [nop,nop,TS val 533959658 ecr 3098599658], length 0
10:41:15.236257 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [.], ack 139, win 85, options [nop,nop,TS val 533959658 ecr 3098599658], length 0
10:41:15.258326 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [P.], seq 1:18, ack 139, win 85, options [nop,nop,TS val 533959658 ecr 3098599658], length 17: HTTP: HTTP/1.0 200 OK
10:41:15.258361 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [P.], seq 1:18, ack 139, win 85, options [nop,nop,TS val 533959658 ecr 3098599658], length 17: HTTP: HTTP/1.0 200 OK
10:41:15.258375 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [FP.], seq 18:348, ack 139, win 85, options [nop,nop,TS val 533959659 ecr 3098599658], length 330: HTTP
10:41:15.258379 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [FP.], seq 18:348, ack 139, win 85, options [nop,nop,TS val 533959659 ecr 3098599658], length 330: HTTP
10:41:15.292973 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [.], ack 18, win 83, options [nop,nop,TS val 3098599752 ecr 533959658], length 0
10:41:15.293005 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [.], ack 18, win 83, options [nop,nop,TS val 3098599752 ecr 533959658], length 0
10:41:15.307015 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [F.], seq 139, ack 349, win 83, options [nop,nop,TS val 3098599771 ecr 533959659], length 0
10:41:15.307050 IP 10.0.1.100.60176 > 192.168.2.2.http: Flags [F.], seq 139, ack 349, win 83, options [nop,nop,TS val 3098599771 ecr 533959659], length 0
10:41:15.371439 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [.], ack 140, win 85, options [nop,nop,TS val 533959765 ecr 3098599771], length 0
10:41:15.371471 IP 192.168.2.2.http > 10.0.1.100.60176: Flags [.], ack 140, win 85, options [nop,nop,TS val 533959765 ecr 3098599771], length 0
10:41:25.369146 IP 172.64.3.10.42594 > 192.168.2.2.33434: UDP, length 32
10:41:25.369183 ARP, Request who-has 172.64.3.10 (Broadcast) tell 172.64.3.1, length 28
10:41:25.386370 IP 172.64.3.10.52051 > 192.168.2.2.33435: UDP, length 32
10:41:25.427751 IP 172.64.3.10.40635 > 192.168.2.2.33436: UDP, length 32
10:41:25.427795 IP 172.64.3.10.55769 > 192.168.2.2.33437: UDP, length 32
10:41:25.427800 ARP, Request who-has 192.168.2.2 (Broadcast) tell 192.168.2.1, length 28
10:41:25.427813 IP 172.64.3.10.38500 > 192.168.2.2.33438: UDP, length 32
10:41:25.427818 IP 172.64.3.10.60534 > 192.168.2.2.33439: UDP, length 32
10:41:25.444478 IP 172.64.3.10.49482 > 192.168.2.2.33440: UDP, length 32
10:41:25.447016 IP 172.64.3.10.57919 > 192.168.2.2.33441: UDP, length 32
10:41:25.465368 IP 172.64.3.10.41883 > 192.168.2.2.33442: UDP, length 32
10:41:25.486403 IP 172.64.3.10.54880 > 192.168.2.2.33443: UDP, length 32
10:41:25.492949 IP 172.64.3.10.58319 > 192.168.2.2.33444: UDP, length 32
10:41:25.510549 IP 172.64.3.10.41820 > 192.168.2.2.33445: UDP, length 32
10:41:25.515221 IP 172.64.3.10.44104 > 192.168.2.2.33446: UDP, length 32
10:41:25.533224 IP 172.64.3.10.52839 > 192.168.2.2.33447: UDP, length 32
10:41:25.537889 IP 172.64.3.10.47008 > 192.168.2.2.33448: UDP, length 32
10:41:25.553875 IP 172.64.3.10.44936 > 192.168.2.2.33449: UDP, length 32
10:41:25.558741 ARP, Reply 172.64.3.10 is-at 36:52:e8:f2:55:88 (oui Unknown), length 28
10:41:25.558778 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:25.558787 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:25.558790 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:25.605946 ARP, Reply 192.168.2.2 is-at be:14:05:22:c9:1f (oui Unknown), length 28
10:41:25.605978 IP 172.64.3.10.44936 > 192.168.2.2.33449: UDP, length 32
10:41:25.605988 IP 172.64.3.10.47008 > 192.168.2.2.33448: UDP, length 32
10:41:25.605992 IP 172.64.3.10.52839 > 192.168.2.2.33447: UDP, length 32
10:41:25.605994 IP 172.64.3.10.44104 > 192.168.2.2.33446: UDP, length 32
10:41:25.605996 IP 172.64.3.10.41820 > 192.168.2.2.33445: UDP, length 32
10:41:25.605998 IP 172.64.3.10.58319 > 192.168.2.2.33444: UDP, length 32
10:41:25.606001 IP 172.64.3.10.54880 > 192.168.2.2.33443: UDP, length 32
10:41:25.606003 IP 172.64.3.10.41883 > 192.168.2.2.33442: UDP, length 32
10:41:25.606004 IP 172.64.3.10.57919 > 192.168.2.2.33441: UDP, length 32
10:41:25.606006 IP 172.64.3.10.49482 > 192.168.2.2.33440: UDP, length 32
10:41:25.606008 IP 172.64.3.10.60534 > 192.168.2.2.33439: UDP, length 32
10:41:25.606010 IP 172.64.3.10.38500 > 192.168.2.2.33438: UDP, length 32
10:41:25.606012 IP 172.64.3.10.55769 > 192.168.2.2.33437: UDP, length 32
10:41:25.606017 IP 172.64.3.10.41994 > 192.168.2.2.33450: UDP, length 32
10:41:25.606021 IP 172.64.3.10.41994 > 192.168.2.2.33450: UDP, length 32
10:41:25.606024 IP 172.64.3.10.36044 > 192.168.2.2.33451: UDP, length 32
10:41:25.606027 IP 172.64.3.10.36044 > 192.168.2.2.33451: UDP, length 32
10:41:25.606030 IP 172.64.3.10.38360 > 192.168.2.2.33452: UDP, length 32
10:41:25.606033 IP 172.64.3.10.38360 > 192.168.2.2.33452: UDP, length 32
10:41:25.782930 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33449 unreachable, length 68
10:41:25.782964 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33449 unreachable, length 68
10:41:25.782977 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33448 unreachable, length 68
10:41:25.782981 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33448 unreachable, length 68
10:41:25.782985 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33447 unreachable, length 68
10:41:25.782988 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33447 unreachable, length 68
10:41:25.782991 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33446 unreachable, length 68
10:41:25.782993 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33446 unreachable, length 68
10:41:25.782996 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33445 unreachable, length 68
10:41:25.782999 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33445 unreachable, length 68
10:41:25.783002 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33444 unreachable, length 68
10:41:25.783005 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33444 unreachable, length 68
10:41:33.204766 IP 172.64.3.10.46021 > 192.168.2.2.33434: UDP, length 32
10:41:33.204805 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:33.222338 IP 172.64.3.10.36572 > 192.168.2.2.33435: UDP, length 32
10:41:33.222383 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:33.241007 IP 172.64.3.10.32986 > 192.168.2.2.33436: UDP, length 32
10:41:33.241044 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:41:33.260520 IP 172.64.3.10.53180 > 192.168.2.2.33437: UDP, length 32
10:41:33.260559 IP 172.64.3.10.53180 > 192.168.2.2.33437: UDP, length 32
10:41:33.260579 IP 172.64.3.10.51287 > 192.168.2.2.33438: UDP, length 32
10:41:33.260583 IP 172.64.3.10.51287 > 192.168.2.2.33438: UDP, length 32
10:41:33.306172 IP 172.64.3.10.44011 > 192.168.2.2.33439: UDP, length 32
10:41:33.306205 IP 172.64.3.10.44011 > 192.168.2.2.33439: UDP, length 32
10:41:33.306221 IP 172.64.3.10.59520 > 192.168.2.2.33440: UDP, length 32
10:41:33.306226 IP 172.64.3.10.59520 > 192.168.2.2.33440: UDP, length 32
10:41:33.306236 IP 172.64.3.10.45682 > 192.168.2.2.33441: UDP, length 32
10:41:33.306241 IP 172.64.3.10.45682 > 192.168.2.2.33441: UDP, length 32
10:41:33.306250 IP 172.64.3.10.47288 > 192.168.2.2.33442: UDP, length 32
10:41:33.306253 IP 172.64.3.10.47288 > 192.168.2.2.33442: UDP, length 32
10:41:33.374597 IP 172.64.3.10.40549 > 192.168.2.2.33443: UDP, length 32
10:41:33.374629 IP 172.64.3.10.40549 > 192.168.2.2.33443: UDP, length 32
10:41:33.374642 IP 172.64.3.10.55809 > 192.168.2.2.33444: UDP, length 32
10:41:33.374645 IP 172.64.3.10.55809 > 192.168.2.2.33444: UDP, length 32
10:41:33.374650 IP 172.64.3.10.52880 > 192.168.2.2.33445: UDP, length 32
10:41:33.374653 IP 172.64.3.10.52880 > 192.168.2.2.33445: UDP, length 32
10:41:33.374656 IP 172.64.3.10.39790 > 192.168.2.2.33446: UDP, length 32
10:41:33.374658 IP 172.64.3.10.39790 > 192.168.2.2.33446: UDP, length 32
10:41:33.374661 IP 172.64.3.10.49258 > 192.168.2.2.33447: UDP, length 32
10:41:33.374664 IP 172.64.3.10.49258 > 192.168.2.2.33447: UDP, length 32
10:41:33.374667 IP 172.64.3.10.33283 > 192.168.2.2.33448: UDP, length 32
10:41:33.374669 IP 172.64.3.10.33283 > 192.168.2.2.33448: UDP, length 32
10:41:33.374673 IP 172.64.3.10.52679 > 192.168.2.2.33449: UDP, length 32
10:41:33.374675 IP 172.64.3.10.52679 > 192.168.2.2.33449: UDP, length 32
10:41:33.488359 IP 172.64.3.10.59953 > 192.168.2.2.33450: UDP, length 32
10:41:33.488393 IP 172.64.3.10.59953 > 192.168.2.2.33450: UDP, length 32
10:41:33.488406 IP 172.64.3.10.57246 > 192.168.2.2.33451: UDP, length 32
10:41:33.488410 IP 172.64.3.10.57246 > 192.168.2.2.33451: UDP, length 32
10:41:33.488415 IP 172.64.3.10.49618 > 192.168.2.2.33452: UDP, length 32
10:41:33.488418 IP 172.64.3.10.49618 > 192.168.2.2.33452: UDP, length 32
10:41:33.488421 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33437 unreachable, length 68
10:41:33.488424 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33437 unreachable, length 68
10:41:33.488427 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33438 unreachable, length 68
10:41:33.488430 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33438 unreachable, length 68
10:41:33.488433 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33439 unreachable, length 68
10:41:33.488435 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33439 unreachable, length 68
10:41:33.488438 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33440 unreachable, length 68
10:41:33.488453 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33440 unreachable, length 68
10:41:33.488456 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33441 unreachable, length 68
10:41:33.488459 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33441 unreachable, length 68
10:41:33.488462 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33442 unreachable, length 68
10:41:33.488465 IP 192.168.2.2 > 172.64.3.10: ICMP 192.168.2.2 udp port 33442 unreachable, length 68
10:47:52.335478 IP 172.64.3.10.53320 > 10.0.2.2.33434: UDP, length 32
10:47:52.335537 ARP, Request who-has 172.64.3.10 (Broadcast) tell 172.64.3.1, length 28
10:47:52.351779 IP 172.64.3.10.58473 > 10.0.2.2.33435: UDP, length 32
10:47:52.369937 IP 172.64.3.10.42115 > 10.0.2.2.33436: UDP, length 32
10:47:52.375043 IP 172.64.3.10.55831 > 10.0.2.2.33437: UDP, length 32
10:47:52.375086 ARP, Request who-has 10.0.2.2 (Broadcast) tell 172.64.3.1, length 28
10:47:52.390865 IP 172.64.3.10.56844 > 10.0.2.2.33438: UDP, length 32
10:47:52.431609 IP 172.64.3.10.49585 > 10.0.2.2.33439: UDP, length 32
10:47:52.431645 IP 172.64.3.10.40527 > 10.0.2.2.33440: UDP, length 32
10:47:52.431651 IP 172.64.3.10.56464 > 10.0.2.2.33441: UDP, length 32
10:47:52.433587 IP 172.64.3.10.45452 > 10.0.2.2.33442: UDP, length 32
10:47:52.439049 IP 172.64.3.10.34732 > 10.0.2.2.33443: UDP, length 32
10:47:52.456324 IP 172.64.3.10.57138 > 10.0.2.2.33444: UDP, length 32
10:47:52.461154 IP 172.64.3.10.49263 > 10.0.2.2.33445: UDP, length 32
10:47:52.476661 IP 172.64.3.10.59512 > 10.0.2.2.33446: UDP, length 32
10:47:52.481216 IP 172.64.3.10.33228 > 10.0.2.2.33447: UDP, length 32
10:47:52.498223 IP 172.64.3.10.35309 > 10.0.2.2.33448: UDP, length 32
10:47:52.504019 IP 172.64.3.10.42911 > 10.0.2.2.33449: UDP, length 32
10:47:52.519087 ARP, Reply 172.64.3.10 is-at 36:52:e8:f2:55:88 (oui Unknown), length 28
10:47:52.519140 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519149 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519153 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519155 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519158 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519160 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519162 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519164 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519167 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519169 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519171 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519173 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519175 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519178 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:47:52.519181 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519183 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:47:52.519185 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.519188 IP 172.64.3.1 > 172.64.3.10: ICMP time exceeded in-transit, length 36
10:47:52.519190 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.601088 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601166 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601176 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601180 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601182 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601185 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601188 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601190 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601195 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601199 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601211 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601213 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.601216 IP 172.64.3.1 > 172.64.3.10: ICMP host 10.0.2.2 unreachable, length 36
10:47:52.798310 IP 172.64.3.10.34550 > 10.0.2.2.33450: UDP, length 32
10:47:52.798368 IP 172.64.3.1 > 172.64.3.10: ICMP net 10.0.2.2 unreachable, length 36
10:47:52.798380 ARP, Request who-has 10.0.2.2 (Broadcast) tell 172.64.3.1, length 28
10:47:53.601329 ARP, Request who-has 10.0.2.2 (Broadcast) tell 172.64.3.1, length 28
```
