
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
# Q&A

## Can this plugin work in showfoto?

No, althrough this plugins be marked as a "Generic Plugin", and it can be load by showFoto, but it
cannot work this time. It's because digikam's API return a empty url list, so this plugins cannot
load any pictures. If possible, I will fix it.

## What's the difference between "Custom Loader" with "Digikam Loader"?

Custom Loader is a image loader that wrote by me, it use QImageReader and QThreadPool to load
pictures. It may has a better perference and less bugs in some scene.

Digikam Loader is a image loader that provided by digikam, the load process are controled by
digikam, it can use digikam's cache and use less CPUs.

In a word, the difference between "Custom Loader" with "Digikam Loader" is:

- If a picture big than 1920x1080, then Custom Loader will scale it to 1920x1080 for reduce the
  occpuation of mermory
- If this plugin closed before all pictures be loaded, "Digikam Loader" may cause digikam crash.
- "Custom Loader" will use full of your CPUs to speed up pictures's load.

## Why some pictures not shown?

It a bug, please try to resize after pictures loaded.
