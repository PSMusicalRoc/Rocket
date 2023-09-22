# Rocket

This is a (very wip) game engine designed mostly by myself. The entity-component system is based on (and much of the code borrowed from) a very helpful guide on the internet linked [here](). Thank you \<name>!

The engine is built on OpenGL and GLFW3. It is possible in the future that I may add Vulkan support, but for now OpenGL is simpler.

### Cloning Instructions

```
git clone https://github.com/PSMusicalRoc/Rocket.git
git submodule init --recursive
```

### Building instructions

First, make sure you've run the above to clone the repository correctly! This is important for the vendor submodules I use!

```
premake5 gmake2
make config=release
```

It will generate a binary in the `bin/Release/` folder.