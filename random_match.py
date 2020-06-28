import random
base_list = ['00', '01', '10', '11']
query = ''
for i in range (0, 31):
    query += random.choice(base_list)
    
print(query)
print(" ")

match_location = []
for i in range (0, 7000): # make xxx reference
    ref = ''
    for j in range (0, 31):
        ref += random.choice(base_list)
    #print(ref)
    matched = 1
    for k in range (0, 62):
        if query[k] != ref[k]:
            matched = 0
            match_location.append(k)
            #print(k)
            break
        
print(max(match_location))
