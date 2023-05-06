from pytesseract import pytesseract
from PIL import Image

aa = Image.open("large_image.PNG")
xx = pytesseract.image_to_string(aa)
print(xx)



# https://linuxhint.com/install-tesseract-windows/