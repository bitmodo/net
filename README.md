# Cod Networking [![Build Status](https://travis-ci.org/codmf/net.svg?branch=master)](https://travis-ci.org/codmf/net)
The Cod networking layer is a simple layer that helps us interface with networking.
It provides a simple and easy API which can be implemented to support different environments.

## Purpose
The purose of the Cod networking layer is to provide a simple, consistent API across all platforms.
There are a lot of different API's on Windows, Linux, etc. that would require vastly complicated implementations.
Rather than spending time trying to work all of those into a project, we opted to create a new API to simplify the experience.

## Inspiration
The inspiration behind this project would be the [Cod Microframework](https://github.com/codmf/cod).
Cod is a microframework written in C, and it aims to not rely on any external dependencies.
This project is a member of the [Cod Microframework Ecosystem](https://github.com/codmf).
That means that it is an official project that is supported and used by Cod.

## Current status
Right now, we are still in extremely early development.
Things aren't expected to actually be working for a while.
Sit tight, and check back in a while!

## Usage
This project uses [Meson](https://mesonbuild.com/) as its build system.
Once you have that installed, you have two options.
You could clone this repository to make some changes, or you could use this project as a dependency.

### Making changes
If you want to make changes, first you need to clone this repository.
Once you have the code, you can run the following command to build the project:

```cmd
meson build
```
This will build the project into the `build` directory.

The rest is a work-in-progress.
Wait for more!

### Using as a dependency
If you want to use this project as a dependency, first you need to have a project.
From there, you can add this repository as a subproject using Meson's [wrap system](https://mesonbuild.com/Wrap-dependency-system-manual.html).
Your wrap file should look something like the following:

```
[wrap-git]
url = https://github.com/codmf/net.git
revision = head
```

The rest is a work-in-progress.
Wait for more!
