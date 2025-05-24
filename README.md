This is an open source determenistic fighting game engine with an embedded scripting language

overview of how most FGs are designed. Functional, but definitely not shippable! This repo is just a reference for folks curious about FG engine architecture

check out updated repos for input handling (dsl uses bytecode instead of currying, bitfields for inputs, and a simpler circular buffer) - https://github.com/MicahMartin/fg_3d

and integration with godot via gdextension - https://github.com/MicahMartin/fg_server_cpp


### Linux

```shell
sudo apt install nlohmann-json3-dev libboost-all-dev
```

GGPO
```shell
git clone --branch linuxpr https://github.com/Shugyousha/ggpo ggpo_linux
cd ggpo_linux
cmake -Bbuild
cmake --build build
cd build && make
ls -lha build/src/libGGPO.so
```

Build

```shell
cmake -Bbuild
cmake --build build
```

Run
```shell
cd build && ./kusoge
```
