# linksprite
C++ class to communicate with the LinkSprite 2MP TTL camera.

Console Application:

    Usage: lscon -p <serialport> [OPTIONS]
    
    Options:
      -h, --help                   Print this help message
      -p, --port=serialport        Serial port to the LinkSprite camera
      -b, --baud=baudrate          Baudrate (bps) of camera (default 115200)

Example (with debug print statements):

    [ksomervi@toto linksprite]$ ./lscon -p /dev/ttyUSB0 
    Opening port: /dev/ttyUSB0 at 115200 baud
    console started ...
    ]> s
    Set image size...
    0: 160x120
    1: 320x240
    2: 640x480
    3: 800x600
    4: 1024x768
    5: 1280x960
    6: 1600x1200
    Select size: 2
    tx: 56 0 54 1 0
    rx: 76 0 54 0 0
    ]> t
    Taking image...
    Take image...
    tx: 56 0 36 1 0
    rx: 76 0 36 0 0
    ]> l
    Read image size...
    tx: 56 0 34 1 0
    rx: 76 0 34 0 4 0 0 65 de
    size: 0x65de 26078
    ]> d
    Read image size...
    tx: 56 0 34 1 0
    rx: 76 0 34 0 4 0 0 65 de
    size: 0x65de 26078
    Download image to ls_img.jpg ...
    ......................................................................................................
    last data packet...
    rx: c3 b5 14 c 4a 5c 50 21 56 a6 a2 e0 1d e9 77 71 40 84 a5 eb 48 62 63 34
    de d4 7a 8 ff d9
    rx: 76 0 32 0 0
    Downloaded 26078 bytes
    ]> q
    Camera port is open
    - Device: /dev/ttyUSB0
    - Baudrate: 115200
    ]> x
    Closing the camera port...
    close_session: closing port /dev/ttyUSB0
    Exitting...

