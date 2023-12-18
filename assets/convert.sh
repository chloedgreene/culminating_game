#!/bin/sh

# reset the bg directory
rm -rf ../nitrofiles/bg
rm -rf ../nitrofiles/sprite
rm -rf ../nitrofiles/fnt
mkdir -p ../nitrofiles/bg
mkdir -p ../nitrofiles/sprite
mkdir -p ../nitrofiles/fnt

######################
## BACKGROUND STUFF ##
######################

# title screen stuff
grit title_screen_top.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit title_screen_bottum.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit signature_minigame_bottum.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
grit blank_background.png -ftb -fh! -gTFF00FF -gt -gB8 -mR8 -mLs



for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done
mv *.pal *.img *.map ../nitrofiles/bg

##################
## SPRITE STUFF ##
##################

#debug sprites
grit debug_16.png -ftb -fh! -gTFF00FF -gt -gB8 -m!
grit pen_sprite.png -ftb -fh! -gTFF00FF -gt -gB8 -m!


for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

mv *.pal *.img ../nitrofiles/sprite

#################
##  FONT SHIT  ##
#################

grit default.png -ftb -fh! -gTFF00FF -gt -gB8 -m!

for file in *.bin; do
    mv -- "$file" "${file%.bin}"
done

for file in *.img; do
    mv -- "$file" "${file%.img}".fnt
done

mv *.pal *.fnt ../nitrofiles/fnt