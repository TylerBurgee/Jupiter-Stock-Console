import requests
from bs4 import BeautifulSoup

class WebScraper:

    def __init__(self, url):
        self.url = url
        
        self.openPrice = 0
        self.currentPrice = 0
        self.change = 0
        self.volume = 0
        self.priceTarget = 0
        self.chartPattern = 0
        
        headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.114 Safari/537.36'}

    # SEARCH URL TO FIND WEB ELEMENTS AND STORE THEIR CONTENTS
    def search(self):
        r = requests.get(self.url)
        requests.Session().get(r.url)
        soup = BeautifulSoup(r.text, 'html.parser')

        # FIND CURRENT PRICE
        try:
            self.currentPrice = soup.find('span', {'class': 'Trsdu(0.3s) Fw(b) Fz(36px) Mb(-4px) D(ib)'}).text
        except:
            self.currentPrice = "Current Price Unavailable"
            
        # FIND OPEN PRICE
        try:
            self.openPrice = soup.find('td', {'data-test': 'OPEN-value'}).text
        except:
            self.openPrice = "Open Price Unavailable"
            
        # CALCULATE PRICE CHANGE
        try:
            self.change = (float(self.currentPrice) - float(self.openPrice))
        except:
            self.change = "Price Change Unavailable"
            
        # FIND VOLUME
        try:
            self.volume = soup.find('td', {'data-test': 'TD_VOLUME-value'}).text
        except:
            self.volume = "Volume Unavailable"
            
        # FIND PRICE TARGET
        try:
            self.priceTarget = soup.find('td', {'data-test': 'ONE_YEAR_TARGET_PRICE-value'}).text
        except:
            self.volume = "Price Target Unavailable"
            
        # FIND CHART PATTERN
        try:
            self.chartPattern = soup.find('div', {'class': 'Fz(xs) Mb(4px)'}).text
            self.chartPattern = self.chartPattern.split("p")
            self.chartPattern = self.chartPattern[0]
        except:
            self.chartPattern = "Chart Pattern Unavailable"

    # PRINT ALL DATA TO CONSOLE
    def showData(self):
        print("------------------------")
        print("Current Price: $" + self.currentPrice)
        print("Open Price: $" + self.openPrice)
        print("Change: $" + str(self.change))
        print("Volume: " + self.volume)
        print("Price Target: $" + self.priceTarget)
        print("Recent Chart Pattern: " + self.chartPattern)
        print("------------------------")

    def getCurrentPrice(self):
        return self.currentPrice

    def getOpenPrice(self):
        return self.openPrice

    def getChange(self):
        return self.change

    def getVolume(self):
        return self.volume

    def getPriceTarget(self):
        return self.priceTarget

    def getChartPattern(self):
        return self.chartPattern
