import os

ret = "# Gallery\n\n"
for filename in os.listdir("."):
    if not ".jpg" in filename:
        continue
    ret += f"## Seed: {filename[:-4]}\n"
    fname = filename.replace(" ","")
    ret += f"![{filename[:-4]}](./{fname})\n\n"

file = open("gallery.md","w")
file.write(ret)
file.close()

"""
# Gallery

## BADDUS
![baddus](./baddus.png)

"""
