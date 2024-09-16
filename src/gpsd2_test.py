import gpsd2

# Connect to the local gpsd
gpsd2.connect()

# Get GPS position
packet = gpsd2.get_current()

print(packet.position())
