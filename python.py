import serial
import psutil
import GPUtil
import time
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume
from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL

devices = AudioUtilities.GetSpeakers()
interface = devices.Activate(
    IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
volume = cast(interface, POINTER(IAudioEndpointVolume))
ser = serial.Serial('COM4', 9600, timeout=1)

time.sleep(2)  # Wait for 2 seconds to give the Arduino time to get ready
while True:
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
    volumePercent = round(volume.GetMasterVolumeLevelScalar() * 100) # type: ignore
    ser.write(f'INFO:CPU:{cpu_usage},MEM_RAW:{memory_usage_raw},MEM_TOTAL:{memory_total},GPU:{gpu_usage},TEMP:{gpu_temp},TIME:{timePrint},VOL:{volumePercent}\n'.encode())

    time.sleep(0.5)  # Wait for 0.5 second before sending the next set of data