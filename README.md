# linksprite
C++ class to communicate with the LinkSprite 2MP TTL camera.

Console Application:

    Usage: lscon -p <serialport> [OPTIONS]
    
    Options:
      -h, --help                   Print this help message
      -p, --port=serialport        Serial port to the LinkSprite camera
      -b, --baud=baudrate          Baudrate (bps) of camera (default 115200)
     lscon - Version 1.2rc

Example (with debug print statements):

    [ksomervi@toto linksprite]$ ./lscon -p /dev/ttyUSB0 
    Opening port: /dev/ttyUSB0 at 115200 baud
    console started ...
    ]> s
    Set image geometry...
    0: 160x120 [cmd: 0x22]
    1: 320x240 [cmd: 0x11]
    2: 640x480 [cmd: 0x0]
    3: 800x600 [cmd: 0x1d]
    4: 1024x768 [cmd: 0x1c]
    5: 1280x960 [cmd: 0x1b]
    6: 1600x1200 [cmd: 0x21]
    Select geometry: 2
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
    rx: 76 0 34 0 4 0 0 6e 70
    image size: 0x6e70 (28272 bytes)
    ]> d
    Read image size...
    tx: 56 0 34 1 0
    rx: 76 0 34 0 4 0 0 6e 70
    Downloading image ...
    image size: 28272 bytes (0x6e70)
      - Packet read size: 32
        - Reading 884 packets
    ..........................................................................
    .....................................
    last data packet...
    rx: 28 a0 2 8a 0 28 a0 2 92 80 a 5a 10 1f ff d9
    rx: 76 0 32 0 0
    Downloaded 884 packets
    Downloaded 28272 bytes
    ]> q
    Camera port is open
      - Device: /dev/ttyUSB0
      - Baudrate: 115200
    ]> x
    Closing the camera port...
    Exitting...

