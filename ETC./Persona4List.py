import requests
from bs4 import BeautifulSoup

r = requests.get("https://megamitensei.fandom.com/wiki/List_of_Persona_4_Personas")
soup = BeautifulSoup(r.content,'html.parser')
table=soup.findAll("table",{"class":"table p4"})

personaList=[]
for t in table:
    person=t.findAll("td")
    for p in person:
        try:
            personaList.append({
                "Name": p.contents[0].attrs['title'],
                "url": "https://megamitensei.fandom.com/"+p.contents[0].attrs['href'],
            })
        except KeyError: pass

for persona in personaList:
    r = requests.get(persona["url"])
    soup = BeautifulSoup(r.content,'html.parser')
    table = soup.findAll("table",{"style":"margin: 0 auto; min-width:650px;text-align:center; background: #222; border:2px solid #FFE600; border-radius:10px; font-size:75%; font-family:verdana;"})[-1]

    del persona["url"]
    atr=[table.text.split("\n")[i] for i in [40,41,  53,54,55,56,57,58]]
    persona["Arcana"]=atr[0]
    persona["Level"]=atr[1]

    persona["Inherit"]=atr[2]
    persona["Reflects"]=atr[3]
    persona["Absorbs"]=atr[4]
    persona["Block"]=atr[5]
    persona["Resists"]=atr[6]
    persona["Weak"]=atr[7]

    if persona["Reflects"] != "-" or persona["Absorbs"] != "-": persona["point"]=3
    elif persona["Block"] != "-": persona["point"]=2
    elif persona["Resists"] != "-": persona["point"]=1
    else: persona["point"]=0

personaList=sorted(personaList, key=lambda k: k['point'])
personaList=sorted(personaList, key=lambda k: k['Level'])

try:
    with open("data", 'w') as csvfile:
        import csv
        writer = csv.DictWriter(csvfile, fieldnames=personaList[0].keys())
        writer.writeheader()
        for data in personaList:
            writer.writerow(data)
except IOError:
    print("I/O error")
