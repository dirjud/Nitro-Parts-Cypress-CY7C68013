import time
import nitro
from nitro_parts.Microchip.M24XX import program_fx2_prom 
import logging
log=logging.getLogger(__name__)


def get_dev(di_file="Cypress/CY7C68013/CY7C68013.xml", serial_num=None, bus_addr=None, VID=0x1fe1, PID=0x8613, timeout=10):
    """Opens a device and loads the DI file. If a serial_num is
       provided, this opens that device. You can also specify the VID
       and PID. This will wait until 'timeout' seconds for a device to
       connect."""

    dev = nitro.USBDevice(VID, PID)
    
    time0 = time.time()
    while time0+timeout > time.time():
        try:
            if not(bus_addr is None):
                dev.open_by_address(bus_addr)
            elif not(serial_num is None):
                dev.open_by_serial(serial_num)
            else:
                dev.open()
            break
        except nitro.Exception:
            if not(bus_addr is None):
                log.info("Waiting for device with VID=0x%x PID=0x%x BUS_ADDR=%d to connect" % (VID,PID,bus_addr))
            elif not(serial_num is None):
                log.info("Waiting for device with VID=0x%x PID=0x%x SERIAL_NUM=%s to connect" % (VID,PID,serial_num))
            else:
                log.info("Waiting for device with VID=0x%x PID=0x%x to connect" % (VID,PID))
            time.sleep(1)
                
    if not(dev.is_open()):
        raise nitro.Exception("Could not open 0x%x:0x%x device" % (VID, PID))
    log.info("Opened 0x%x:0x%x device with serial number %s" % (VID, PID, dev.get_serial()))

    init_dev(dev, di_file)

    return dev

def init_dev(dev, di_file="Cypress/CY7C68013/CY7C68013.xml"):
    """Initializes the device with the di files."""

    di = nitro.load_di(di_file)
    dev.set_di(di)

def program_fx2(dev, filename):
    """
        Program the fx2 with the firmware ihx file.  filename should be set to
        the path of the ihx file.  This function causes the fx2 to reboot and
        renumerate with the USB bus.  The device is automatically closed and 
        must be re-opened after it has reconnected to the Host.
    """
    log.info("Programming FX2 with iic file %s" % filename)
    f=open(filename,'rb').read()
#    print type(dev)
#    dev=nitro.USBDevice(dev) # ensure load_firmware available
    dev.load_firmware ( f )
    log.info ("Firmware loaded, device automatically closed." )

def program_new_pcb(fx2_firmware, VID, PID, di_file, fx2_prom_term='FX2_PROM'):
    """
        This function does not require an open device.  It looks
        for the 1st unprogrammed pcb (by using the default Cypress
        Vendor ID/Product ID and attempts to load the fx2 firmware
        files specified.

        :param fx2_firmware: The fx2 firmare ihx file. 
        :param vendor id of firmware being loaded.
        :param product id of firmware being loaded.
        :param path to di file implemented by firmware.
    """
    dev=nitro.USBDevice(0x04b4,0x8613)
    dev.open(0,True)
    program_fx2(dev,fx2_firmware)
    time.sleep(1)

    # 
    while nitro.USBDevice.get_device_count(VID, PID) < 1:
        time.sleep(1)

    dev=nitro.USBDevice(VID, PID)
    dev.open()
    dev.set_di( nitro.load_di ( di_file ) )
    program_fx2_prom(dev,fx2_firmware, fx2_prom_term)
    dev.close()

