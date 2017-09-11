mv data{,.old} && echo 'Moved old data to data.old'
if [ -f ~/Dropbox/GameDev/oyunum/data.tar.gz ]; then
    cp ~/Dropbox/GameDev/oyunum/data.tar.gz .
else
    wget https://www.dropbox.com/s/ystc9qq6dqxy2nu/data.tar.gz?dl=0 -O data.tar.gz
fi
tar xf data.tar.gz && echo 'Extracted tar archive to data directory.' && rm -v data.tar.gz && echo 'Removed tar archive' && rm -rf data.old && echo 'Deleted data.old'
