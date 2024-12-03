systemctl restart networking

# Exp1:
ifconfig eth1 up
ifconfig eth1 172.16.30.1/24
# ifconfig # Register IP and MAC addresses
# route -n # Inspect Forwarding
# arp -a # Inspect ARP tables
# arp -d 172.16.30.254 # Delete entry for TUX34 (not mandatory for configuration)
# Start Wrireshark on eth1
# ping 172.16.30.254 # Ping TUX34

# Exp2:
# Start Wrireshark on eth1
# ping 172.16.30.254
# ping 172.16.31.1
# Start Wrireshark on eth1. Also start it on eth1 of TUX32 and TUX34
# ping -b 172.16.30.255 # Ping broadcast

# Exp3:
route add -net 172.16.30.1/24 gw 172.16.30.254
# route -n # Observe routes
# Start Wrireshark on eth1.
# ping 172.16.30.254 # Ping and verify connectivity
# ping 172.16.31.253 # Ping and verify connectivity
# ping 172.16.31.1 # Ping and verify connectivity
# Start Wrireshark on eth1 AND eth2 of TUX34.
# ping 172.16.31.1 # Ping and sabe logs of TUX34
# arp -d 172.16.30.254 # Delete entry for TUX34 (not mandatory for configuration)

# Exp4:
# route -n # Verify the default route (0.0.0.0) is TUX34
# TODO
