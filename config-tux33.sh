systemctl restart networking

# Exp1:
ifconfig eth1 up
ifconfig eth1 172.16.50.1/24
# ifconfig # Register IP and MAC addresses
# route -n # Inspect Forwarding
# arp -a # Inspect ARP tables
# arp -d 172.16.50.254 # Delete entry for TUX54 (not mandatory for configuration)
# Start Wrireshark on eth1
# ping 172.16.50.254 # Ping TUX54

# Exp2:
# Start Wrireshark on eth1
# ping 172.16.50.254
# ping 172.16.51.1
# Start Wrireshark on eth1. Also start it on eth1 of TUX52 and TUX54
# ping -b 172.16.50.255 # Ping broadcast

# Exp3:
route add -net 172.16.51.0/24 gw 172.16.50.254
# route -n # Observe routes
# Start Wrireshark on eth1.
# ping 172.16.50.254 # Ping and verify connectivity
# ping 172.16.51.253 # Ping and verify connectivity
# ping 172.16.51.1 # Ping and verify connectivity
# Start Wrireshark on eth1 AND eth2 of TUX54.
# ping 172.16.51.1 # Ping and sabe logs of TUX54
# arp -d 172.16.50.254 # Delete entry for TUX54 (not mandatory for configuration)

# Exp4:
route add -net 172.16.1.0/24 gw 172.16.50.254

# Exp5:
# echo domain netlab.fe.up.pt > /etc/resolv.conf
# echo search netlab.fe.up.pt >> /etc/resolv.conf
echo nameserver 10.227.20.3 >> /etc/resolv.conf
ping ftp.netlab.fe.up.pt
