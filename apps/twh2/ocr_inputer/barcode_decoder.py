
# Importing library
import cv2
from pyzbar.pyzbar import decode   # pip install pyzbar   https://github.com/NaturalHistoryMuseum/pyzbar/issues/93
  
# Make one method to decode the barcode
def BarcodeReader(image_file):
     

    # read the image in numpy array using cv2
    img = cv2.imread(image_file)
      
    # Decode the barcode image
    detectedBarcodes = decode(img)
    return
    

    # If not detected then print the message
    if not detectedBarcodes:
        print("Barcode Not Detected or your barcode is blank/corrupted!")
    else:
       
          # Traverse through all the detected barcodes in image
        for barcode in detectedBarcodes: 
           
            # Locate the barcode position in image
            (x, y, w, h) = barcode.rect
             
            # Put the rectangle in image using
            # cv2 to highlight the barcode
            cv2.rectangle(img, (x-10, y-10),
                          (x + w+10, y + h+10),
                          (255, 0, 0), 2)
             
            if barcode.data!="":
               
            # Print the barcode data
                print(barcode.data)
                print(barcode.type)
                 
    #Display the image
    cv2.imshow("Image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
 
# mark_image = cv2.imread('mark_image.png')
# img_file="barcode.png"

# # read the image in numpy array using cv2
# img = cv2.imread(img_file)

if __name__ == "__main__":
    
  # Take the image from user
    # image="D:/temp/barcode2.JPG"
    file_name ="barcode.png"
    BarcodeReader(file_name)