import requests
from bs4 import BeautifulSoup
import time
import urllib


gameID=str(730)
gameName='csgo'

nameList=[]
fastitemPrice=[]
steamPrice=[]
steamVolume=[]

maxPrice=str(95000)
minPrice=str(20000)
arz_usdToT=27.5
dMarket=0.35
buffer=0.22

# get data from cash market
for page in range(0, 50):
    page = str(page)
    payload="CurrentPage="+page+"&Hero=null&Type=null&Rarity=null&MinPrice="+minPrice+"&MaxPrice="+maxPrice+"&SearchItem=&PriceSort=0&steamid=nist&Appid="+gameName+"&CuOptions=undefined&isoffer=undefined&PageNav=yes"
    headers = {
      'content-type': 'application/x-www-form-urlencoded; charset=UTF-8',
      'Cookie': 'PHPSESSID=a3dvtpabuekvp3oqglpe8e9sd3'
    }
    response = requests.request("POST", "https://fastitem.ir/request/", headers=headers, data=payload)
    soup = BeautifulSoup(response.content, 'html.parser')
    preLen=len(nameList)
    for response in soup.findAll("p", {"class":"title"}):
        nameList.append(response.text)
    for response in soup.findAll("div", {"class":"price"}):
        dictForChange=["تومان",":","قیمت"," ","\n","\r",","]
        price=response.text
        for p in dictForChange: price=price.replace(p,"")
        fastitemPrice.append(float(price)/1000)
    if preLen==len(nameList): break

# get data from steam market
dMarket-=0.03+buffer
print((1-dMarket)*arz_usdToT)
for i, response in enumerate(nameList):
    if response[:1]==" ": response=response[1:]
    if response[-1:]==" ": response=response[:-1]
    response=urllib.parse.quote(response)
    soup = requests.get("https://steamcommunity.com/market/priceoverview/?appid="+gameID+"&currency=1&market_hash_name="+response.replace(" ", "%20")).json()
    while soup is None:
        soup = requests.get("https://steamcommunity.com/market/priceoverview/?appid="+gameID+"&currency=1&market_hash_name="+response.replace(" ", "%20")).json()
        time.sleep(5)
    if soup["success"]:
        if "median_price" in soup.keys() and "lowest_price" in soup.keys() and float(soup["median_price"][1:])<=1.10*float(soup["lowest_price"][1:]):
            print("reich")
            steamPrice.append(float(soup["median_price"][1:])*0.9)
        if "median_price" in soup.keys():
            steamPrice.append(float(soup["median_price"][1:])*0.9)
            print("ge")
        else:
            steamPrice.append(float(soup["lowest_price"][1:])*0.9)
            print("ja")
        if "volume" in soup.keys():
            steamVolume.append(soup["volume"])
        else:
            steamVolume.append(None)
    else:
        print(response)
        steamPrice.append(None)
        steamVolume.append(None)

    # *********************************************
    name, sPrice, fPrice, sVol = nameList[i], steamPrice[i], fastitemPrice[i], steamVolume[i]
    if sPrice!=None and fPrice <= sPrice*(1-dMarket)*arz_usdToT:
        print(name, sVol, fPrice, sPrice*(1-dMarket)*arz_usdToT)
    else: print("no")

