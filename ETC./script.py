import pysubs2
from sys import argv
from googletrans import Translator


def transliterate(text):
    translator = Translator()
    return translator.translate(text, src='ja', dest='ja').pronunciation


def makeFile(filename):
    orig = pysubs2.load(filename, encoding="utf-8-sig")
    romanji = pysubs2.load(filename, encoding="utf-8-sig")
    n=25
    symb="- | -"
    for i in range(0,len(orig)-n,n):
        if i%100==0: print(i)
        lines=orig[i].text
        for j in range(1,n):
            lines += symb+orig[i+j].text
        lines=transliterate(lines)
        lines=lines.split(symb)
        for j in range(n):
            romanji[i+j].text = lines[j]
            
    i+=1-n
    lines=orig[i].text
    for j in range(1,len(orig)-i):
        lines += symb+orig[i+j].text
    lines=transliterate(lines)
    lines=lines.split(symb)
    for j in range(len(orig)-i):
        romanji[i+j].text = lines[j]
            
    romanji.save(filename[:-4]+'_ramaji_'+filename[-4:])


for i in range(1, len(argv)):
    makeFile(argv[i])