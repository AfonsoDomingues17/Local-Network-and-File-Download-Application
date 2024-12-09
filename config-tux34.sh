systemctl restart networking

# Exp1:
ifconfig eth1 up
ifconfig eth1 172.16.50.254/24
# ifconfig # Register IP and MAC addresses
# route -n # Inspect Forwarding
# arp -a # Inspect ARP tables

# Exp3:
ifconfig eth2 up
ifconfig eth2 172.16.51.253/24
sysctl net.ipv4.ip_forward=1
sysctl net.ipv4.icmp_echo_ignore_broadcasts=0
# ifconfig # Register IP and MAC addresses of eth1 and eth2
# route -n # Observe routes
# Start Wrireshark on eth1 AND eth2. On TUX53, ping TUX52
# arp -d 172.16.51.1 # Delete entry for TUX52 (not mandatory for configuration)
# arp -d 172.16.50.1 # Delete entry for TUX53 (not mandatory for configuration)

# Exp4:
route add -net 172.16.1.0/24 gw 172.16.51.254
# TODO


# Exp5:
echo domain netlab.fe.up.pt > /etc/resolv.conf
echo search netlab.fe.up.pt >> /etc/resolv.conf
echo nameserver 10.227.20.3 >> /etc/resolv.conf
