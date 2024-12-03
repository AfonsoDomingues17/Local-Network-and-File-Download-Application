systemctl restart networking

# Exp2:
ifconfig eth1 up
ifconfig eth1 172.16.31.1/24
# Start Wrireshark on eth1. Also start it on eth1 of TUX32 and TUX34
# ping -b 172.16.31.255 # Ping broadcast

# Exp3:
route add -net 172.16.31.1/24 gw 172.16.31.253
# route -n # Observe routes
# arp -d 172.16.31.253 # Delete entry for TUX34 (not mandatory for configuration)

# Exp4:
# route -n # Verify the default route (0.0.0.0) is ROUTER (172.16.31.254)
# TODO
