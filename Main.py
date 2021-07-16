from WebScraper import WebScraper
from DataComputations import DataComputations
import time
import os
import serial

class Main:

    def __init__(self):
        self.ser = serial.Serial('/dev/cu.usbmodem14601',9600)
        self.count = 0
        self.tickerList = []
        self.disallowedCharacters = "\r\n"
        
    # SEND SERIAL DATA TO ARDUINO
    def sendData(self, ticker):
        tickerScraper = WebScraper('https://finance.yahoo.com/quote/' + ticker)
        tickerScraper.search()
        tickerScraper.showData()

        # SEND MARKET VALUE
        self.ser.write(str(tickerScraper.getCurrentPrice()))

        # SEND OPEN PRICE POINT
        self.ser.write("_" + str(tickerScraper.getOpenPrice()))

        # SEND PRICE CHANGE
        self.ser.write("_" + str(tickerScraper.getChange()))

        # SEND VOLUME
        self.ser.write("_" + str(tickerScraper.getVolume()))

        # SEND PRICE TARGET
        self.ser.write("_" + str(tickerScraper.getPriceTarget()))

        # SEND RECENT CHART PATTERN
        self.ser.write("_" + str(tickerScraper.getChartPattern()) + "_^")
        
        time.sleep(0.5)

    # READ SERIAL DATA FROM ARDUINO
    def arduinoSearch(self):
        search = False
        
        while (search == False):
            msg = self.ser.readline()
            decoded_msg = str(msg[0:len(msg)-2].decode("utf-8"))
            if (decoded_msg == "search"):
                print("searching...")
                print("")
                search = True
            elif (decoded_msg == "clear"):
                self.tickerList = []
                print("cleared")
                print("")
            else:
                self.tickerList.append(str(msg))
                print(msg)
                self.count += 1

        tickerString = ''.join(self.tickerList)

        for character in self.disallowedCharacters:
            tickerString = tickerString.replace(character, "")

        print(tickerString)

        self.sendData(tickerString)

# MAIN INSTANCE OF THE TradeConsole CLASS
TradeConsole = Main()

# START THE PROGRAM
TradeConsole.arduinoSearch()
