import gpsd2

# Connect to the local gpsd
gpsd2.connect()

device = gpsd2.device()
print(device)

try:
    # Get GPS position
    packet = gpsd2.get_current()
    print(packet.position())
except gpsd2.NoFixError as err:
    print("NoFixError:", err)
