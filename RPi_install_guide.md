INSTRUKCJA INSTALACJI RPI

//włączenie remote ssh
sudo systemctl enable ssh
sudo systemctl start ssh

sudo apt-get update
sudo apt-get install -y vim vim-nox git python3-smbus i2c-tools python3-pip supervisor

//ułatwienie życia
alias py="python3"
alias ll="ls -lah"

// zmiana ustawień klawiatury na pl
sudo sed -i -e 's/gb/pl/g' /etc/default/keyboard

//aktwacja i2c
sudo echo 'i2c-dev' | sudo tee -a /etc/modules
sudo sed -i -e 's/#dtparam=i2s=on/dtparam=i2s=on/g' /boot/config.txt
sudo sed -i -e 's/#dtparam=i2c_arm=on/dtparam=i2c_arm=on/g' /boot/config.txt

// pobranie repo
git clone https://repo.inovatica.com/apakula/programiaki.git

//instalacja nvm
wget -qO- https://raw.githubusercontent.com/creationix/nvm/v0.33.8/install.sh | bash
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"

nvm install 7.10.1
nvm use 7.10.1

// podgranie elementów by serwisy działały poprawnie
sudo pip3 install websockets
cd /home/pi/programiaki/websockets
npm install
cp /home/pi/programiaki/rfid_reader/app.py /home/pi/o.py

//podgranie plików do supervisor 
sudo cp /home/pi/programiaki/supervisor/* /etc/supervisor/conf.d/

//ustawianie read only
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/read-only-fs.sh
sudo bash read-only-fs.sh 

Enable boot-time read/write jumper? [y/N] - YES na pin GPIO21
Install GPIO-halt utility? [y/N] - NO
Enable kernel panic watchdog? [y/N] - YES
--- Choose RPI version ---
Continue - yes

REBOOT - YES

//po przełączeniu na ReadOnly giną logi supervisor'a więc musimy je utworzyć:
    sudo mkdir /var/log/supervisor
    sudo touch /var/log/supervisor/supervisor.log
    sudo chmod -R 777 /var/log/supervisor
    sudo service supervisor restart
