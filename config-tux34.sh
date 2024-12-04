systemctl restart networking

# Exp1:
ifconfig eth1 up
ifconfig eth1 172.16.30.254/24
# ifconfig # Register IP and MAC addresses
# route -n # Inspect Forwarding
# arp -a # Inspect ARP tables

# Exp3:
ifconfig eth2 up
ifconfig eth2 172.16.31.253/24
sysctl net.ipv4.ip_forward=1
sysctl net.ipv4.icmp_echo_ignore_broadcasts=0
# ifconfig # Register IP and MAC addresses of eth1 and eth2
# route -n # Observe routes
# Start Wrireshark on eth1 AND eth2. On TUX33, ping TUX32
# arp -d 172.16.31.1 # Delete entry for TUX32 (not mandatory for configuration)
# arp -d 172.16.30.1 # Delete entry for TUX33 (not mandatory for configuration)

# Exp4:
# route -n # Verify the default route (0.0.0.0) is ROUTER (172.16.31.254)
# TODO
