
# Importing library
import cv2
from pyzbar.pyzbar import decode   # pip install pyzbar   https://github.com/NaturalHistoryMuseum/pyzbar/issues/93
  
# Make one method to decode the barcode
last_barcode_value = ""

def BarcodeReader(image):
    global last_barcode_value
    # Decode the barcode image
    detectedBarcodes = decode(image)

    if detectedBarcodes:
        # Traverse through all the detected barcodes in image
        for barcode in detectedBarcodes: 
           
            # Locate the barcode position in image
            (x, y, w, h) = barcode.rect
             
            # Put the rectangle in image using
            # cv2 to highlight the barcode
            cv2.rectangle(image, (x-10, y-10),
                          (x + w+10, y + h+10),
                          (255, 0, 0), 2)
             
            if barcode.data!="":
                if last_barcode_value != barcode.data:
                    print(barcode.data)
                    print(barcode.type)
                    last_barcode_value = barcode.data
                
    else:
        # blank image, did not detect any barcode
        pass                 
    #Display the image
    cv2.imshow("Image", image)
    cv2.waitKey(1)


if __name__ == "__main__":
    test_from_file = False
    test_from_camera = True

    if test_from_file:    
        file_name ="barcode.png"
        img = cv2.imread(file_name)
        BarcodeReader(img)

    if test_from_camera:
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Cannot open camera")
            exit()

        while True:
            ret, frame = cap.read()
            if ret:
                BarcodeReader(frame)