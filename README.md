
# Explain

This is a Digikam plugin for add more beautiful flow layout.

# Screenshot

![628863199077](screenshot/1.gif)
![1628863199079](./screenshot/缩放.gif)
![1628863199080](./screenshot/节省内存.gif)

# Build

This project depends these components (Fedora):

- digikam-devel
- qt5-qtbase-devel
- c++ 17
- CMake

To build and install this, please run:

```bash
git checkout https://github.com/cathaysia/digikamflowplugin.git
git submodule update --init
./bootstrap.sh
cd build
make
sudo make install/fast
```

