import sensor, time
from machine import UART
#import image

uart = UART(1,115200)
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1000)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # can be QVGA on M7...
sensor.skip_frames(200)
sensor.set_auto_gain(False) # must turn this off to prevent image washout...


while(True):

            while(True):
                img = sensor.snapshot()
                img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
                codes = img.find_qrcodes()
                if (codes):
                    uart.write(codes[0].payload())
                    print(codes[0].payload())

                    time.sleep(1)

                    break;
                #else:
                    #print("fail!")
