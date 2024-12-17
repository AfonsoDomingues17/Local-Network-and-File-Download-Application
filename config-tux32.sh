systemctl restart networking

# Exp2:
ifconfig eth1 up
ifconfig eth1 172.16.51.1/24
# Start Wrireshark on eth1. Also start it on eth1 of TUX52 and TUX54
# ping -b 172.16.51.255 # Ping broadcast

# Exp3:
route add -net 172.16.50.0/24 gw 172.16.51.253
# route -n # Observe routes
# arp -d 172.16.51.253 # Delete entry for TUX54 (not mandatory for configuration)

# Exp4:
route add -net 172.16.1.0/24 gw 172.16.51.254
# sysctl net.ipv4.conf.eth1.accept_redirects=0
# sysctl net.ipv4.conf.all.accept_redirects=0

#route del -net 172.16.50.0/24 gw 172.16.51.253
#route add -net 172.16.50.0/24 gw 172.16.51.254
#traceroute 172.16.50.1
#route del -net 172.16.50.0/24 gw 172.16.51.254
#route add -net 172.16.50.0/24 gw 172.16.51.253
#traceroute 172.16.50.1
#sysctl net.ipv4.conf.eth1.accept_redirects=1
#sysctl net.ipv4.conf.all.accept_redirects=1


# Exp5:
# echo domain netlab.fe.up.pt > /etc/resolv.conf
# echo search netlab.fe.up.pt >> /etc/resolv.conf
echo nameserver 10.227.20.3 >> /etc/resolv.conf
