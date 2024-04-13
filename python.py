import serial
import psutil
import GPUtil
import time
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
from PIL import Image
import pystray
import threading
import logging

logging.basicConfig(filename='C:\\Users\\natha\\Documents\\GitHub\\arduino-serial-monitor\\log.txt', level=logging.ERROR)

try:
    run = True
    display = True


    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(
        IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume = cast(interface, POINTER(IAudioEndpointVolume))


    ser = serial.Serial('COM4', 9600, timeout=1)

    def create_menu():
        return pystray.Menu(
            pystray.MenuItem('Quit', on_quit_callback),
            pystray.MenuItem('Reboot', on_reboot_callback),
            pystray.MenuItem(lambda text: 'Display Off' if display else 'Display On', on_display_off_callback)
        )

    def on_quit_callback(icon, item):
        global run
        icon.stop()
        ser.write('SHUTDOWN\n'.encode())
        run = False

    def on_display_off_callback(icon, item):
        global display
        display = False if display else True

    def on_reboot_callback(icon, item):
        ser.write('REBOOT\n'.encode())


    image = Image.open('C:\\Users\\natha\\Documents\\GitHub\\arduino-serial-monitor\\icon.png')
    icon = pystray.Icon('System Monitor', image, 'System Monitor', menu=create_menu())
    icon_thread = threading.Thread(target=icon.run)
    icon_thread.start()

    time.sleep(2)  # Wait for 2 seconds to give the Arduino time to get ready

    while run:
        gpus = GPUtil.getGPUs()
        gpu = gpus[0]

        cpu_usage = max(psutil.cpu_percent(interval=1, percpu=True))
        memory = psutil.virtual_memory()
        memory_usage = memory.percent
        memory_usage_raw = round(memory.used / (1024 ** 3), 2)
        memory_total = round(memory.total / (1024 ** 3))
        gpu_usage = round(gpu.load * 100)
        gpu_temp = gpu.temperature

        timePrint = time.strftime('%H:%M', time.localtime())

        volumePercent = round(volume.GetMasterVolumeLevelScalar() * 100)

        if ser.is_open:
            if display:
                ser.write(f'INFO:CPU:{cpu_usage},MEM_RAW:{memory_usage_raw},MEM_TOTAL:{memory_total},GPU:{gpu_usage},TEMP:{gpu_temp},TIME:{timePrint},VOL:{volumePercent}\n'.encode())
            else:
                ser.write('DISPLAY_OFF\n'.encode())
        else:
            print('Serial port is not open')
        
        #if ser.in_waiting > 0:
        #    try:
        #        
        #    except UnicodeDecodeError:
        #        pass  # Ignore the error
    #
        time.sleep(0.5)  # Wait for 0.5 second before sending the next set of data
except Exception as e:
    logging.exception(e)