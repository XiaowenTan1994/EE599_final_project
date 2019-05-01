import torch
from torch.autograd import Variable
import utils
import dataset
from PIL import Image

import models.crnn as crnn


model_path = './data/v2.pth'
root = './data/Fnt/Sample'
alphabet = '0123456789abcdefghijklmnopqrstuvwxyz'

model = crnn.CRNN(32, 1, 37, 256)
if torch.cuda.is_available():
    model = model.cuda()
#print('loading pretrained model from %s' % model_path)
model = torch.nn.DataParallel(model)
model.load_state_dict(torch.load(model_path, map_location='cpu'))
f=open('result3.txt','w')
for i in range(1,63):
   path = root+str(i).zfill(3)+'/'
   for j in range(1,101):
      converter = utils.strLabelConverter(alphabet)
      transformer = dataset.resizeNormalize((100, 32))
      img_path = path+'img'+str(i).zfill(3)+'-'+str(j).zfill(5)+'.png'
      image = Image.open(img_path).convert('L')
      image = transformer(image)
      if torch.cuda.is_available():
         image = image.cuda()
      image = image.view(1, *image.size())
      image = Variable(image)

      model.eval()
      preds = model(image)

      _, preds = preds.max(2)
      preds = preds.transpose(1, 0).contiguous().view(-1)

      preds_size = Variable(torch.IntTensor([preds.size(0)]))
      raw_pred = converter.decode(preds.data, preds_size.data, raw=True)
      sim_pred = converter.decode(preds.data, preds_size.data, raw=False)
      print('%-20s => %-20s' % (raw_pred, sim_pred))
      #f.write(path)
      #f.write(',')
      f.write(sim_pred)
      f.write('\n')
