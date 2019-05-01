f=open('result1.txt', 'r')
dif = 0
content = f.readlines()
content = [x.strip() for x in content] 

for i in range(1,63):
   for j in range(1,101):
      r = content[(i-1)*100+j-1]
      if i <=10:
         if r != i:
            dif = dif+1
      else:
         if r != str(chr(97+(i-11)%26)):  
            dif = dif+1 

print ("accuracy of origianl model: %f" % (1-(float(dif)/len(content))))

f=open('result2.txt', 'r')
dif = 0
content = f.readlines()
content = [x.strip() for x in content] 

for i in range(1,63):
   for j in range(1,101):
      r = content[(i-1)*100+j-1]
      if i <=10:
         if r != i:
            dif = dif+1
      else:
         if r != str(chr(97+(i-11)%26)):  
            dif = dif+1 
print ("accuracy of finetuned model: %f" % (1-(float(dif)/len(content))))
