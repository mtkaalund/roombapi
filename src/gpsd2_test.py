import gpsd2

# Connect to the local gpsd
gpsd2.connect()

# Get GPS position
packet = gpsd2.get_current()

device = gpsd2.device()

print(device)

try:
    print(packet.position())
except NoFixError as err:
    print("NoFixError:", err)
