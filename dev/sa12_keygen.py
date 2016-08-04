import random

ProductEdition = 9
MaxProcessors = 31
HighAvailability = 1
InMemory = 1
MaxCopyNodes = 1
ProcessorLic = 0

OC = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"

k = [0, ] * 25

for i in range(0x0B, len(k)):
    k[i] = random.randint(24, 31)


k[0x00] = OC.find('Q')
k[0x01] = OC.find('E')
k[0x02] = MaxProcessors
k[0x03] = OC.find('F')
k[0x04] = OC.find('E')
k[0x05] = (ProcessorLic << 4) | ProductEdition
k[0x06] = 0b11111
k[0x07] = 0b11111
k[0x08] = 0b11111
k[0x09] = 0b11111
k[0x0A] = (HighAvailability << 4) | (InMemory << 3) | (MaxCopyNodes << 2)

#sum
k[0x0B] = ( 0x1F  + 1 - (sum(k[:0x0B]) & 0x1F) ) & 0x1F

ks = ""
for i in range(0, len(k)):
    if i and i % 5 == 0:
        ks += '-'
    ks += OC[ k[i] ]
    
print ks

