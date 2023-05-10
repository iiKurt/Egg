# Importing Image class from PIL module
from PIL import Image
 
# Opens a image in RGB mode
im = Image.open(r"./32x32.png")
 
# Size of the image in pixels (size of original image)
# (This is not mandatory)
width, height = im.size

dimensions = 32
result = dimensions
iteration = 0

working = True

while working:
	iteration += 1
	result = result + dimensions

	if result > 1024:
		break

	print(str(result) + "	(*" + str(iteration) + ")")

	newsize = (result, result)
	im1 = im.resize(newsize, Image.NEAREST)
	# Shows the image in image viewer
	#im1.show()
	im1.save("./" + str(result) + "x" + str(result) + ".png", "PNG")
