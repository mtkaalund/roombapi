# roombapi
Roomba PI, for controlling my ROOBA with a Raspberry PI Zero 2

Remember to add user to ```dialout```.

```sudo adduser ${USER} dialout```

## Create a python vent
First clone the project then in the project folder use the following command
```python3 -m vent env```

Then activate the enviroment
```source env/bin/activate```

Now we can install the requirements for the project
```pip install -r requirements.txt```

To deactivate the enviroment
```deactivate```


## Projects used in this project
[py-gpsd2](https://github.com/hatsunearu/py-gpsd2)
[RPLCD](https://pypi.org/project/RPLCD/)

## Other resources
[medium.com how to set up a RPI4 with LCD](https://medium.com/@thedyslexiccoder/how-to-set-up-a-raspberry-pi-4-with-lcd-display-using-i2c-backpack-189a0760ae15)
[RPLCD RTD.io](https://rplcd.readthedocs.io/en/stable/getting_started.html)
[GPSD austinsnerdythings.com](https://austinsnerdythings.com/2021/04/19/microsecond-accurate-ntp-with-a-raspberry-pi-and-pps-gps/)
[RPI GPS satsignal](https://www.satsignal.eu/ntp/Raspberry-Pi-quickstart.html)
