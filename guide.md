# Guide for Lab 2 Network Configuration

## Beginning
Restart networking in all the tuxes:
```bash
systemctl restart networking
```

Restart the **switch** and the **router**:
```
/system reset-configuration
``` 

The user is **admin** and the password is blank.

## Exp 1

Connect eth1 of tuxY3 and tuxY4 to the switch (ether3 & ether4).

### tuxY3
Configure eth1 interface:
```bash
ifconfig eth1 up
ifconfig eth1 172.16.Y0.1/24
```

### tuxY4
Configure eth1 interface:
```bash
ifconfig eth1 up
ifconfig eth1 172.16.Y0.254/24
```

## Exp 2

Connect eth1 of tuxY2 to the switch (ether2).

### tuxY2
Configure eth1 interface:
```bash
ifconfig eth1 up
ifconfig eth1 172.16.Y1.1/24
```

### Switch
Remove ports from default bridge:  
Ether2:
```
/interface bridge port print
/interface bridge port remove
```
Ether3:
```
/interface bridge port print
/interface bridge port remove
```
Ether4:
```
/interface bridge port print
/interface bridge port remove
```

Create bridges **bridgeY0** and **bridgeY1**:
```
/interface bridge add name=bridge50
/interface bridge add name=bridge51
```

Assuming tuxY2, tuxY3, and tuxY4 are connected to ports ether2, ether3 and ether4, respectively. Add ports to bridges:
```
/interface bridge port add bridge=bridge50 interface=ether3
/interface bridge port add bridge=bridge50 interface=ether4
/interface bridge port add bridge=bridge51 interface=ether2
```

## Exp 3

Connect eth2 of tuxY4 to the switch (ether10).

### tuxY4
Configure eth2 interface:
```bash
ifconfig eth2 up
ifconfig eth2 172.16.Y1.253/24
```

Enable IP forwarding:
```bash
sysctl net.ipv4.ip_forward=1
```

Disable ICMP echo-ignore-broadcast:
```bash
sysctl net.ipv4.icmp_echo_ignore_broadcasts=0
```

### Switch

Assuming eth2 interface of tuxY4 is connected to the switch on ether10.
Remove ether10 from default bridge:
```
/interface bridge port remove 
```

Add eth2 of tuxY4 to bridgeY1:
```
/interface bridge port add bridge=bridge51 interface=ether10
```

### tuxY2

Add route to subnetwork 172.16.Y0.0 via eth2 of tuxY4:
```bash
route add -net 172.16.Y0.0/24 gw 172.16.Y1.253
```

### tuxY3

Add route to subnetwork 172.16.Y1.0 via eth1 of tuxY4:
```bash
route add -net 172.16.Y1.0/24 gw 172.16.Y0.254
```

## Exp 4

**IMPORTANT: Reset router, if not done yet!!**

Connect ether1 of Rc to PY.12.
Connect ether2 of RC to the switch (ether15).

### Switch

Assuming ether2 of Rc is connected to the switch on ether15.
Remove ether15 from default bridge:
```
/interface bridge port remove 
```

Add ether2 of Rc to bridgeY1:
```
/interface bridge port add bridge=bridge51 interface=ether15
```

### Router

Configure IP addresses of Rc:
```
/ip address add address=172.16.1.Y1/24 interface=ether1
/ip address add address=172.16.Y1.254/24 interface=ether2
```

### tuxY3

Route to subnetwork 172.16.Y1.0 is already configured in Exp 3.

Add route to 172.16.1.0/24:
```bash
route add -net 172.16.1.0/24 gw 172.16.Y0.254
```

### tuxY4

Add route to 172.16.1.0/24:
```bash
route add -net 172.16.1.0/24 gw 172.16.Y1.254
```

### tuxY2

Route to subnetwork 172.16.Y0.0 is already configured in Exp 3.

Add route to 172.16.1.0/24:
```bash
route add -net 172.16.1.0/24 gw 172.16.Y1.254
```

### Router

Add route to bridgeY0:
```
/ip route add dst-address=172.16.Y0.0/24 gateway=172.16.Y1.253
```

## Exp 5

### tuxY2

Configure the DNS (with ip address of 10.225.20.3):
```bash
echo nameserver 10.227.20.3 >> /etc/resolv.conf
```

### tuxY3

Configure the DNS (with ip address of 10.225.20.3):
```bash
echo nameserver 10.227.20.3 >> /etc/resolv.conf
```

### tuxY4

Configure the DNS (with ip address of 10.225.20.3):
```bash
echo nameserver 10.227.20.3 >> /etc/resolv.conf
```

## Exp 6

Compile the download application:
```bash
gcc download.c -o download
```

Run the download application:
```bash
./download <URL>
```

This URL is in the format ftp://[\<user>:\<password>@]\<host>/\<url-path>

