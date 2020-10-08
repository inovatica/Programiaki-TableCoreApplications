#!/usr/bin/env python3

import smbus, subprocess, os

class Reader():
	def __init__(self, addr):
        self.addr = addr

    def findBests(self, vals):
        m = 0
        result = 0
        for key, value in vals.items():
            if key == "times":
                continue

	        if value > m:
	            m = value
	            result = key
	    return result

	def writeBus(self, value):
	    try:
	        bus.write_byte(self.address,value)
	    except IOError:
	        subprocess.call(['i2cdetect', '-y', '1'],stdout=open(os.devnull,'wb'))
	        writeBus(value)

	def readBus(self, **kwargs):
	    count = kwargs.pop('counter',0)
	    vals = kwargs.pop('values',{})
	    vals["times"] = 0

        if(count > 10):
            raise Exception('Read error more than 10')

	    try:
	        for i in range(0,10):
	            respond =  bus.read_byte(self.address)
	            if respond in vals:
	                vals[respond] += 1
	            else:
	                vals[respond] = 1

	            vals["times"] += 1
	            if(vals["times"] > 9):
	                return findBests(vals)

	    except OSError:
	        count += 1
	        subprocess.call(['i2cdetect', '-y', '1'],stdout=open(os.devnull,'wb'))
	        return readBus(address, counter=count, valuesa=vals)

	def cleanArduinoMemory(self):
		writeBus(68)

	def getCount(self):
		writeBus(70)
		return readBus()

	def getChange(self):
			# get card number (0..n-1)
	        writeBus(74)
	        cardNumber = readBus()
	        
	        # get tagId length
	        writeBus(76)
	        length = readBus()

	        # get first tag's number
	        writeBus(78)
	        tagId = ''
	        for i in range(0, length):
	            tagId += str(readBus(addr))
	            # get next tag's number
	            writeBus(addr, 88)
	        
	        return {"cardNumber": cardNumber, "tagId": tagId}

	def proceed(self)
	    smthNew = readBus()
	    if(smthNew == 20):
	    	return getChange()
	    else:
	    	return false