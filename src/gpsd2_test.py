import gpsd2

# Connect to the local gpsd
gpsd2.connect()

device = gpsd2.device()

print("GPS Device:")
print("\tpath:", device["path"])
print("\tspeed:", device["speed"])
print("\tdriver:", device["driver"])

try:
    # Get GPS position
    packet = gpsd2.get_current()

    print("Properties:")
    print("\tMode:", packet.mode)
    print("\tSatellites:", packet.sats)
    if packet.mode >= 2:
        print("\t\tLatitude:", packet.lat)
        print("\t\tLongitude:", packet.lon)
        print("\t\tTrack:", packet.track)
        print("\t\tHorizontal Speed:", packet.hspeed)
        print("\t\tTime:", packet.time)
        print("\t\tError:", packet.error)
    else:
        print("\t\tLatitude: Not Available")
        print("\t\tLongitude: Not Available")
        print("\t\tTrack: Not Available")
        print("\t\tHorizontal Speed: Not Available")
        print("\t\tError: Not Available")

    if packet.mode >= 3:
        print("\t\tAltitude:", packet.alt)
        print("\t\tClimb:", packet.climb)
    else:
        print("\t\tAltitude: Not Available")
        print("\t\tClimb: Not Available")

    print("Methods:")
    if packet.mode >= 2:
        print("\tLocation:", packet.position())
        print("\tSpeed:", packet.speed())
        print("\tPosition Precision:", packet.position_precision())
#        print("\t\tTime UTC:", packet.time_utc())
        print("\t\tTime Local:", packet.time_local())
        print("\t\tMap URL:", packet.map_url())
    else:
        print("\tLocation: Not Available")
        print("\tSpeed: Not Available")
        print("\tPosition Precision: Not Available")
 #       print("\t\tTime UTC: Not Available")
        print("\t\tTime Local: Not Available")
        print("\t\tMap URL: Not Available")

    if packet.mode >= 3:
        print("\tAltitude:", packet.altitude())
    else:
        print("\tAltitude: Not Available")


except gpsd2.NoFixError as err:
    print("NoFixError:", err)
