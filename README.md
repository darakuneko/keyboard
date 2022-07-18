# memo

QMK fork merge with tags from origin
mkdir ~/Downloads/qmk
cd ~/Downloads/qmk
git clone https://github.com/darakuneko/qmk_firmware.git
cd qmk_firmware
git remote add upstream https://github.com/qmk/qmk_firmware.git
git fetch upstream
git merge upstream/master
git push origin --tags

