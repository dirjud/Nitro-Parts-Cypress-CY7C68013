
import nitro
from nitro_parts.Microchip.M24XX import program_fx2_prom 

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

