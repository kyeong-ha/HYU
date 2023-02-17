input_= """what are you doing
doing how are dowing
do you wanna doing"""

input__ = raw_input()
input__ = input__.lower()
count = 0

for i in range(0,len(input_)) :
    if input__[:] == input_[i:i+len(input__)] :
        count += 1
        
print count
    
